/*
  config.hpp

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
#ifndef _FCOPSS_CONFIG_HPP_
#define _FCOPSS_CONFIG_HPP_

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/ini_parser.hpp>
#include <boost/optional.hpp>

#include <fcopss/common.hpp>

namespace fcopss {

class Config
{
public:
  Config();

  // CLIENT section
  //
  string
  clientProtocol() const;

  time_duration
  clientSubSendInterval() const;

  // ROUTER section
  //
  uint16_t
  routerPort() const;

  uint16_t
  routerControlPort() const;

  time_duration
  routerStExpireTime() const;

  // RP section
  uint16_t
  rpPort() const;

  // TCP section
  //
  time_duration
  tcpConnectionTimeout() const;

  time_duration
  tcpReceiveTimeout() const;

  // for primitive access
  //
  const boost::property_tree::ptree&
  propertyTree() const;

private:
  boost::property_tree::ptree m_ptree;
};

} // namespace fcopss

#endif // _FCOPSS_CONFIG_HPP_ 
