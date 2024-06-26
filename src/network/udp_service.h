/* 
 *  File: udp_service.h
 *  Project: apsdk
 *  Created: Oct 25, 2018
 *  Author: Sheen Tian
 *  
 *  This file is part of apsdk (https://github.com/air-display/apsdk-public) 
 *  Copyright (C) 2018-2024 Sheen Tian 
 *  
 *  apsdk is free software: you can redistribute it and/or modify it under the terms 
 *  of the GNU General Public License as published by the Free Software Foundation, 
 *  either version 3 of the License, or (at your option) any later version.
 *  
 *  apsdk is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; 
 *  without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. 
 *  See the GNU General Public License for more details.
 *  
 *  You should have received a copy of the GNU General Public License along with Foobar. 
 *  If not, see <https://www.gnu.org/licenses/>.
 */

#pragma once
#include <memory>
#include <thread>

#include <asio.hpp>

#include <utils/logger.h>
#include <utils/utils.h>

namespace aps {
namespace network {
class udp_service {
public:
  virtual ~udp_service(){};

  virtual const uint16_t port() const = 0;

  virtual bool open() = 0;
  ;

  virtual void close() = 0;

  virtual asio::io_context &io_context() = 0;
};

typedef std::shared_ptr<udp_service> udp_service_ptr;

typedef std::shared_ptr<asio::ip::udp::endpoint> udp_endpoint_ptr;

class udp_service_base : public udp_service {
public:
  udp_service_base(const std::string &name, uint16_t port = 0)
      : service_name_(name), io_context_(), io_work_(io_context_), socket_(io_context_),
        local_endpoint_(asio::ip::udp::v6(), port), worker_thread_(0) {}

  ~udp_service_base() { close(); }

  virtual const uint16_t port() const override { return local_endpoint_.port(); }

  virtual bool open() override {
    // Create the worker thread
    worker_thread_ = create_aps_thread([&]() {

      io_context_.run();

    });

    if (!worker_thread_)
      return false;

    // Create the acceptor
    socket_.open(local_endpoint_.protocol());
    socket_.set_option(asio::ip::v6_only(false));
    socket_.bind(local_endpoint_);
    local_endpoint_ = socket_.local_endpoint();

    return true;
  }

  virtual void close() override {
    io_context_.stop();

    if (worker_thread_) {
      worker_thread_->join();
      worker_thread_.reset();
    }
  }

  virtual void post_send_to(std::vector<uint8_t> buf, asio::ip::udp::endpoint remote_endpoint) {
    socket_.async_send_to(
        asio::buffer(buf.data(), buf.size()),
        remote_endpoint,
        std::bind(&udp_service_base::on_send_to, this, remote_endpoint, std::placeholders::_1, std::placeholders::_2));
  }

  virtual void post_send_to(uint8_t *buf, size_t length, asio::ip::udp::endpoint remote_endpoint) {
    socket_.async_send_to(
        asio::buffer(buf, length),
        remote_endpoint,
        std::bind(&udp_service_base::on_send_to, this, remote_endpoint, std::placeholders::_1, std::placeholders::_2));
  }

  virtual void on_send_to(asio::ip::udp::endpoint remote_endpoint, const asio::error_code &e,
                          std::size_t bytes_transferred) {}

  virtual void post_recv_from(std::vector<uint8_t> &buf, asio::ip::udp::endpoint &remote_endpoint) {
    socket_.async_receive_from(
        asio::buffer(buf.data(), buf.size()),
        remote_endpoint,
        std::bind(
            &udp_service_base::on_recv_from, this, remote_endpoint, std::placeholders::_1, std::placeholders::_2));
  }

  virtual void post_recv_from(uint8_t *buf, size_t length, asio::ip::udp::endpoint &remote_endpoint) {
    socket_.async_receive_from(
        asio::buffer(buf, length),
        remote_endpoint,
        std::bind(
            &udp_service_base::on_recv_from, this, remote_endpoint, std::placeholders::_1, std::placeholders::_2));
  }

  virtual void on_recv_from(asio::ip::udp::endpoint &remote_endpoint, const asio::error_code &e,
                            std::size_t bytes_transferred) {}

  virtual asio::io_context &io_context() override { return io_context_; }

private:
  std::string service_name_;
  asio::io_context io_context_;
  asio::io_context::work io_work_;
  asio::ip::udp::socket socket_;
  asio::ip::udp::endpoint local_endpoint_;
  aps_thread worker_thread_;
};
} // namespace network
} // namespace aps
