/*
 *  File: ap_config.h
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

#ifndef AP_CONFIG_H
#define AP_CONFIG_H
#pragma once

#include <cmath>
#include <cstdint>
#include <memory>
#include <string>

#include <ap_export.h>

#if _WIN32
#pragma warning(disable : 4251)
#endif

#define DECLARE_STRING_PROPERTY(n)                                                                                     \
private:                                                                                                               \
  std::string n##_;                                                                                                    \
                                                                                                                       \
public:                                                                                                                \
  const std::string &n() const { return n##_; }                                                                        \
                                                                                                                       \
public:                                                                                                                \
  void n(const std::string &value) { n##_ = value; }

#define DECLARE_BOOL_PROPERTY(n)                                                                                       \
private:                                                                                                               \
  bool n##_;                                                                                                           \
                                                                                                                       \
public:                                                                                                                \
  const bool &n() const { return n##_; }                                                                               \
                                                                                                                       \
public:                                                                                                                \
  void n(const bool &value) { n##_ = value; }

#define DECLARE_INTEGER32_PROPERTY(n)                                                                                  \
private:                                                                                                               \
  std::int32_t n##_;                                                                                                   \
                                                                                                                       \
public:                                                                                                                \
  const std::int32_t &n() const { return n##_; }                                                                       \
                                                                                                                       \
public:                                                                                                                \
  void n(const std::int32_t &value) { n##_ = value; }

#define DECLARE_INTEGER64_PROPERTY(n)                                                                                  \
private:                                                                                                               \
  std::int64_t n##_;                                                                                                   \
                                                                                                                       \
public:                                                                                                                \
  const std::int64_t &n() const { return n##_; }                                                                       \
                                                                                                                       \
public:                                                                                                                \
  void n(const std::int64_t &value) { n##_ = value; }

#define DECLARE_FLOAT_PROPERTY(n)                                                                                      \
private:                                                                                                               \
  std::float_t n##_;                                                                                                   \
                                                                                                                       \
public:                                                                                                                \
  const std::float_t &n() const { return n##_; }                                                                       \
                                                                                                                       \
public:                                                                                                                \
  void n(const std::float_t &value) { n##_ = value; }

#define DECLARE_OBJECT_PROPERTY(n, t)                                                                                  \
private:                                                                                                               \
  t n##_;                                                                                                              \
                                                                                                                       \
public:                                                                                                                \
  const t &n() const { return n##_; }                                                                                  \
                                                                                                                       \
public:                                                                                                                \
  void n(const t &value) { n##_ = value; }

namespace aps {
/// <summary>
///
/// </summary>
class APS_EXPORT ap_config_audio_format {
public:
  /// <summary>
  ///
  /// </summary>
  ap_config_audio_format();

  /// <summary>
  ///
  /// </summary>
  ~ap_config_audio_format();

  /// <summary>
  ///
  /// </summary>
  DECLARE_INTEGER32_PROPERTY(audioInputFormats);

  /// <summary>
  ///
  /// </summary>
  DECLARE_INTEGER32_PROPERTY(audioOutputFormats);

  /// <summary>
  ///
  /// </summary>
  DECLARE_INTEGER32_PROPERTY(type);
};

/// <summary>
///
/// </summary>
class APS_EXPORT ap_config_audio_latency {
public:
  /// <summary>
  ///
  /// </summary>
  ap_config_audio_latency();

  /// <summary>
  ///
  /// </summary>
  ~ap_config_audio_latency();

  /// <summary>
  ///
  /// </summary>
  DECLARE_STRING_PROPERTY(audioType);

  /// <summary>
  ///
  /// </summary>
  DECLARE_INTEGER32_PROPERTY(inputLatencyMicros);

  /// <summary>
  ///
  /// </summary>
  DECLARE_INTEGER32_PROPERTY(outputLatencyMicros);

  /// <summary>
  ///
  /// </summary>
  DECLARE_INTEGER32_PROPERTY(type);
};

/// <summary>
///
/// </summary>
class APS_EXPORT ap_config_display {
public:
  /// <summary>
  ///
  /// </summary>
  ap_config_display();

  /// <summary>
  ///
  /// </summary>
  ~ap_config_display();

  /// <summary>
  ///
  /// </summary>
  DECLARE_INTEGER32_PROPERTY(width);

  /// <summary>
  ///
  /// </summary>
  DECLARE_INTEGER32_PROPERTY(height);

  /// <summary>
  ///
  /// </summary>
  DECLARE_FLOAT_PROPERTY(refreshRate);

  /// <summary>
  ///
  /// </summary>
  DECLARE_STRING_PROPERTY(uuid);
};

/// <summary>
///
/// </summary>
class APS_EXPORT ap_config {
public:
  /// <summary>
  ///
  /// </summary>
  static std::shared_ptr<ap_config> default_instance();

  /// <summary>
  ///
  /// </summary>
  ap_config();

  /// <summary>
  ///
  /// </summary>
  ~ap_config();

  /// <summary>
  ///
  /// </summary>
  DECLARE_STRING_PROPERTY(name);

  /// <summary>
  ///
  /// </summary>
  DECLARE_BOOL_PROPERTY(publishService);

  /// <summary>
  ///
  /// </summary>
  DECLARE_STRING_PROPERTY(model);

  /// <summary>
  ///
  /// </summary>
  DECLARE_STRING_PROPERTY(serverVersion);

  /// <summary>
  ///
  /// </summary>
  DECLARE_STRING_PROPERTY(macAddress);

  /// <summary>
  ///
  /// </summary>
  DECLARE_STRING_PROPERTY(pi);

  /// <summary>
  ///
  /// </summary>
  DECLARE_STRING_PROPERTY(pk);

  /// <summary>
  ///
  /// </summary>
  DECLARE_STRING_PROPERTY(flags);

  /// <summary>
  ///
  /// </summary>
  DECLARE_STRING_PROPERTY(audioCodecs);

  /// <summary>
  ///
  /// </summary>
  DECLARE_STRING_PROPERTY(encryptionTypes);

  /// <summary>
  ///
  /// </summary>
  DECLARE_STRING_PROPERTY(metadataTypes);

  /// <summary>
  ///
  /// </summary>
  DECLARE_STRING_PROPERTY(needPassword);

  /// <summary>
  ///
  /// </summary>
  DECLARE_STRING_PROPERTY(transmissionProtocol);

  /// <summary>
  ///
  /// </summary>
  DECLARE_INTEGER32_PROPERTY(vv);

  /// <summary>
  ///
  /// </summary>
  DECLARE_INTEGER64_PROPERTY(features);

  /// <summary>
  ///
  /// </summary>
  DECLARE_INTEGER32_PROPERTY(statusFlag);

  /// <summary>
  ///
  /// </summary>
  DECLARE_OBJECT_PROPERTY(audio_format, ap_config_audio_format);

  /// <summary>
  ///
  /// </summary>
  DECLARE_OBJECT_PROPERTY(audio_latency, ap_config_audio_latency);

  /// <summary>
  ///
  /// </summary>
  DECLARE_OBJECT_PROPERTY(display, ap_config_display);

  /// <summary>
  ///
  /// </summary>
  const std::string features_hex_string() const;

  /// <summary>
  ///
  /// </summary>
  /// <returns></returns>
  const std::string deviceID() const;
};

typedef std::shared_ptr<ap_config> ap_config_ptr;
}; // namespace aps

#endif // AP_CONFIG_H
