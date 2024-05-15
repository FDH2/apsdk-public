﻿#include <ctime>
#include <memory>

#include <ap_config.h>
#include <ap_server.h>
#include <mdns/net_service.h>
#include <service/ap_airplay_service.h>
#include <service/ap_casting_media_data_store.h>
#include <service/ap_casting_media_http_service.h>

using namespace aps::service;
using namespace aps::network;

namespace aps {
class ap_server::implementation {
public:
  implementation()
      : airplay_net_service_("_airplay._tcp"), raop_net_service_("_raop._tcp"), airplay_tcp_service_(0),
        ap_casting_media_http_service_(0) {}

  ~implementation() { release_net_service(); }

  void set_config(ap_config_ptr &config) { ap_config_ = config; }

  void set_handler(ap_handler_ptr handler) { ap_handler_ = handler; }

  bool start() {
    if (airplay_tcp_service_)
      return true;

    airplay_tcp_service_ = std::make_shared<ap_airplay_service>(ap_config_, 0);
    if (!airplay_tcp_service_)
      return false;

    airplay_tcp_service_->set_handler(ap_handler_);

    if (!airplay_tcp_service_->start()) {
      airplay_tcp_service_.reset();
      return false;
    } else {
      LOGD() << "AP service running on " << airplay_tcp_service_->port();
    }

    ap_casting_media_http_service_ = std::make_shared<ap_casting_media_http_service>(ap_config_, 0);

    if (!ap_casting_media_http_service_->start()) {
      LOGW() << "Failed to start media service";
    } else {
      ap_casting_media_data_store::get().set_store_root(ap_casting_media_http_service_->port());
      LOGD() << "Media service running on " << ap_casting_media_http_service_->port();
    }

    if (!initialize_net_service()) {
      airplay_tcp_service_->stop();
      airplay_tcp_service_.reset();
      return false;
    }

    return true;
  }

  void stop() {
    release_net_service();

    if (airplay_tcp_service_) {
      airplay_tcp_service_->stop();
      airplay_tcp_service_.reset();
    }
  }

  uint16_t get_service_port() {
    if (airplay_tcp_service_) {
      return airplay_tcp_service_->port();
    }
    return -1;
  }

protected:
  bool initialize_net_service() {
    if (!ap_config_->publishService()) {
      return true;
    }

    airplay_net_service_.add_txt_record("deviceId", ap_config_->macAddress());
    printf("airplay_net_service deviceID %s\n",ap_config_->macAddress().c_str());
    airplay_net_service_.add_txt_record("features", ap_config_->features_hex_string());
    printf("airplay_net_service features %s\n",ap_config_->features_hex_string().c_str());
    airplay_net_service_.add_txt_record("model", ap_config_->model());
    printf("airplay_net_service model %s\n",ap_config_->model().c_str());
    airplay_net_service_.add_txt_record("srcvers", ap_config_->serverVersion());
    printf("airplay_net_service srcvers %s\n",ap_config_->serverVersion().c_str());
    airplay_net_service_.add_txt_record("vv", ap_config_->vv());
    printf("airplay_net_service vv %d\n",ap_config_->vv());
    airplay_net_service_.add_txt_record("pi", ap_config_->pi());
    printf("airplay_net_service pi %s\n",ap_config_->pi().c_str());
    airplay_net_service_.add_txt_record("pk", ap_config_->pk());
    printf("airplay_net_service pk %s\n",ap_config_->pk().c_str());
    airplay_net_service_.add_txt_record("flags", ap_config_->flags());
    printf("airplay_net_service flags %s\n",ap_config_->flags().c_str());
    
    raop_net_service_.add_txt_record("am", ap_config_->model());
    printf("raop_net_service am %s\n",ap_config_->model().c_str());
    raop_net_service_.add_txt_record("cn", ap_config_->audioCodecs());
    printf("raop_net_service cn %s\n",ap_config_->audioCodecs().c_str());
    raop_net_service_.add_txt_record("et", ap_config_->encryptionTypes());
    printf("raop_net_service et %s\n",ap_config_->encryptionTypes().c_str());
    raop_net_service_.add_txt_record("ft", ap_config_->features_hex_string());
    printf("raop_net_service ft %s\n",ap_config_->features_hex_string().c_str());
    raop_net_service_.add_txt_record("md", ap_config_->metadataTypes());
    printf("raop_net_service md %s\n",ap_config_->metadataTypes().c_str());
    raop_net_service_.add_txt_record("pk", ap_config_->pk());
    printf("raop_net_service pk %s\n",ap_config_->pk().c_str());
    raop_net_service_.add_txt_record("tp", ap_config_->transmissionProtocol());
    printf("raop_net_service tp %s\n",ap_config_->transmissionProtocol().c_str());
    raop_net_service_.add_txt_record("vs", ap_config_->serverVersion());
    printf("raop_net_service vs %s\n",ap_config_->serverVersion().c_str());
    raop_net_service_.add_txt_record("vv", ap_config_->vv());
    printf("raop_net_service vv %d\n",ap_config_->vv());
    raop_net_service_.add_txt_record("vn", "65537");
    printf("raop_net_service vn %s\n","65537");
    raop_net_service_.add_txt_record("da", "true");
    printf("raop_net_service da %s\n","true");
    raop_net_service_.add_txt_record("sf", "0x04");
    printf("raop_net_service sf %s\n","0x04");

    std::string airplay_service_name = ap_config_->name();
    if (airplay_net_service_.publish(airplay_service_name, airplay_tcp_service_->port())) {
      std::string raop_name = ap_config_->deviceID();
      raop_name += "@";
      raop_name += airplay_service_name;

      if (raop_net_service_.publish(raop_name, airplay_tcp_service_->port()))
        return true;

      airplay_net_service_.suppress();
    }

    return false;
  }

  void release_net_service() {
    if (!ap_config_->publishService()) {
      return;
    }

    airplay_net_service_.suppress();
    raop_net_service_.suppress();
  }

private:
  ap_config_ptr ap_config_;

  ap_handler_ptr ap_handler_;

  net_service airplay_net_service_;

  net_service raop_net_service_;

  ap_airplay_service_ptr airplay_tcp_service_;

  ap_casting_media_http_service_ptr ap_casting_media_http_service_;
};

ap_server::ap_server() : impl_(new implementation()) {}

ap_server::~ap_server() {
  if (impl_) {
    delete impl_;
    impl_ = nullptr;
  }
}

void ap_server::set_config(ap_config_ptr &config) { impl_->set_config(config); }

void ap_server::set_handler(ap_handler_ptr &handler) { impl_->set_handler(handler); }

bool aps::ap_server::start() { return impl_->start(); }

void ap_server::stop() { impl_->stop(); }

uint16_t ap_server::get_service_port() { return impl_->get_service_port(); }

#if __ANDROID__
void ap_server::setJavaVM(JavaVM *vm) { setGlobalJavaVM(vm); }
#endif
} // namespace aps
