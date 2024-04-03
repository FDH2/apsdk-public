﻿#include <service/ap_casting_media_data_store.h>
#include <service/ap_casting_media_http_service.h>
#include <stdio.h>

namespace aps {
namespace service {

ap_casting_media_http_connection::ap_casting_media_http_connection(asio::io_context &io_ctx)
    : xtxp_connection_base(io_ctx) {
  printf("ap_casting_media_http_connection::ap_casting_media_http_connection\n");  
  initialize_request_handlers();
}

ap_casting_media_http_connection::~ap_casting_media_http_connection() = default;

void ap_casting_media_http_connection::get_handler(const request &req, response &res) {
  DUMP_REQUEST_WITH_CONNECTION(req);
  printf("ap_casting_media_http_connection::get_handler\n");
  
  auto data = ap_casting_media_data_store::get().query_media_data(req.uri);
  if (data.empty()) {
    res.with_status(not_found);
    return;
  }

  res.with_status(ok).with_content_type(APPLICATION_MPEGURL).with_content(data);
}

void ap_casting_media_http_connection::add_common_header(const request &req, response &res) {
  printf("ap_casting_media_http_connection::add_common_header\n");
  res.with_header(HEADER_DATE, gmt_time_string())
      .with_header(HEADER_ALLOW_HEADER, HEADER_CONTENT_TYPE)
      .with_header(HEADER_ALLOW_ORIGIN, "*");
}

#define RH(x) std::bind(&ap_casting_media_http_connection::x, this, std::placeholders::_1, std::placeholders::_2)

void ap_casting_media_http_connection::initialize_request_handlers() {
  printf("ap_casting_media_http_connection::initialize_request_handlers\n");
  request_route_t routes_table[] = {
      {"HTTP", "GET", "*", RH(get_handler)},
  };
  for (const auto& route : routes_table) {
    register_request_route(route);
  }
}

std::shared_ptr<xtxp_connection_base> ap_casting_media_http_connection::shared_from_self() {
  return shared_from_this();
}

ap_casting_media_http_service::ap_casting_media_http_service(ap_config_ptr &config, uint16_t port /*= 0*/)
    : tcp_service_base("ap_casting_media_http_service", port), config_(config) {}

ap_casting_media_http_service::~ap_casting_media_http_service() = default;

network::tcp_connection_ptr ap_casting_media_http_service::prepare_new_connection() {
  return std::make_shared<ap_casting_media_http_connection>(io_context());
}

} // namespace service
} // namespace aps
