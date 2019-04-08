/*
  config.cpp

  Copyright (c) 2019 KOZO KEIKAKU ENGINEERING Inc.

  This software is released under the MIT License

  Permission is hereby granted, free of charge, to any person obtaining a copy
  of this software and associated documentation files (the "Software"), to deal
  in the Software without restriction, including without limitation the rights
  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
  copies of the Software, and to permit persons to whom the Software is
  furnished to do so, subject to the following conditions:

  The above copyright notice and this permission notice shall be included in all
  copies or substantial portions of the Software.

  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
  SOFTWARE.
*/
#include <fcopss/common.hpp>
#include <fcopss/config.hpp>


namespace fcopss {

using namespace boost::property_tree;

Config::Config()
{
  const char* dir = getenv("FCOPSS_CONF_DIR");
  string path = "fcopss.conf";

  if (dir != nullptr) {
    path = string(dir) + "/" + path;
  }  

  try {
    read_ini(path, m_ptree);
  } catch (ini_parser::ini_parser_error& e) {
  }
}

string
Config::clientProtocol() const
{
  return m_ptree.get("CLIENT.Protocol", "tcp");
}

time_duration
Config::clientSubSendInterval() const
{
  time_duration duration(boost::posix_time::pos_infin);

  if (boost::optional<long> value = m_ptree.get_optional<long>("CLIENT.SubSendInterval")) {
    if (value.get() > 0) {
      duration = boost::posix_time::seconds(value.get());
    }
  }

  return duration;
}

uint16_t
Config::routerPort() const
{
  return m_ptree.get("ROUTER.Port", uint16_t(9876));
}

uint16_t
Config::routerControlPort() const
{
  return m_ptree.get("ROUTER.ControlPort", uint16_t(9877));
}

time_duration
Config::routerStExpireTime() const
{
  time_duration duration(boost::posix_time::pos_infin);

  if (boost::optional<long> value = m_ptree.get_optional<long>("ROUTER.StExpireTime")) {
    if (value.get() > 0) {
      duration = boost::posix_time::seconds(value.get());
    }
  }

  return duration;
}

uint16_t
Config::rpPort() const
{
  return m_ptree.get("RP.Port", uint16_t(9878));
}

time_duration
Config::tcpConnectionTimeout() const
{
  time_duration duration(boost::posix_time::pos_infin);

  if (boost::optional<long> value = m_ptree.get_optional<long>("TCP.ConnectionTimeout")) {
    if (value.get() > 0) {
      duration = boost::posix_time::seconds(value.get());
    }
  }

  return duration;
}

time_duration
Config::tcpReceiveTimeout() const
{
  time_duration duration(boost::posix_time::pos_infin);

  if (boost::optional<long> value = m_ptree.get_optional<long>("TCP.ReceiveTimeout")) {
    if (value.get() > 0) {
      duration = boost::posix_time::seconds(value.get());
    }
  }

  return duration;
}

const ptree&
Config::propertyTree() const
{
  return m_ptree;
}

} // namespace fcopss
