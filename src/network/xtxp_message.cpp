/* 
 *  File: xtxp_message.cpp
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

#include <network/xtxp_message.h>

#define DUMP_REQUEST_BODY 1

namespace aps {
namespace network {
xtxp_message::xtxp_message(const std::string &scm_ver) : scheme_version(scm_ver), content_length(0) {}

xtxp_message::xtxp_message() {}

xtxp_message::~xtxp_message() {}

xtxp_message &xtxp_message::with_content_type(const std::string &type) {
  content_type = type;
  return *this;
}

xtxp_message &xtxp_message::with_content(const std::string &data) {
  std::copy(data.begin(), data.end(), std::back_inserter(content));
  content_length = data.length();
  return *this;
}

xtxp_message &xtxp_message::with_content(const std::vector<uint8_t> data) {
  content = data;
  content_length = data.size();
  return *this;
}

xtxp_message &xtxp_message::with_content(const uint8_t *data, int length) {
  std::copy(data, data + length, std::back_inserter(content));
  content_length = length;
  return *this;
}

xtxp_message &xtxp_message::with_header(const std::string &name, const std::string &value) {
  headers[name] = value;
  return *this;
}

std::string xtxp_message::serialize() const {
  std::ostringstream oss;
  for (auto &kv : headers)
    oss << kv.first << CHAR_COLON << CHAR_BLANK << kv.second << RN_LINE_BREAK;

  oss << HEADER_CONTENT_LENGTH << CHAR_COLON << CHAR_BLANK << content_length << RN_LINE_BREAK;

  if (!content_type.empty())
    oss << HEADER_CONTENT_TYPE << CHAR_COLON << CHAR_BLANK << content_type << RN_LINE_BREAK;

  oss << RN_LINE_BREAK;

  std::copy(content.begin(), content.end(), std::ostream_iterator<uint8_t>(oss));
  return oss.str();
}

request::request(const std::string &scheme_ver, const std::string &methot, const std::string &uri)
    : xtxp_message(scheme_ver), method(methot), uri(uri) {}

request::request() {}

std::string request::serialize() const {
  std::vector<uint8_t> result;
  std::ostringstream oss;
  oss << method << CHAR_BLANK << uri << CHAR_BLANK << scheme_version << RN_LINE_BREAK;
  oss << xtxp_message::serialize();
  return oss.str();
}

void request::dump(const std::string tag) const {
  std::ostringstream oss;
  oss << tag << "<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< " << std::endl
      << "    Request: " << method << " " << uri << " " << scheme_version << std::endl
      << "    Header:" << std::endl;
  for (auto &header : headers)
    oss << "        " << header.first << ": " << header.second << std::endl;

#ifdef DUMP_REQUEST_BODY
  oss << "    Body:";
  if (0 == content_length)
    oss << "<EMPTY>";

  for (int i = 0; i < content_length; i++) {
    if (std::isprint(content[i]))
      oss << content[i];
    else
      oss << '.';
  }
#endif
  LOGD() << oss.str();
}

response::response(const std::string &scheme_ver)
    : xtxp_message(scheme_ver), status_code(status_type_t::ok), status_text("OK") {}

response::response() : status_code(status_type_t::ok), status_text("OK") {}

response &response::with_status(status_type_t code) {
  status_code = code;
  auto code_string = g_status_code_sting_map.find(code);
  if (code_string != g_status_code_sting_map.end()) {
    status_text = code_string->second;
  } else {
    status_text = "No Description";
  }

  return *this;
}

std::string response::serialize() const {
  std::ostringstream oss;
  oss << scheme_version << CHAR_BLANK << status_code << CHAR_BLANK << status_text << RN_LINE_BREAK;
  oss << xtxp_message::serialize();
  return oss.str();
}

} // namespace network
} // namespace aps
