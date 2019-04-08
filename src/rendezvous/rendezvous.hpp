/*
  rendezvous.hpp

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
#ifndef _FCOPSS_RENDEZVOUS_RENDEZVOUS_HPP_
#define _FCOPSS_RENDEZVOUS_RENDEZVOUS_HPP_

#include <fcopss/common.hpp>
#include <fcopss/config.hpp>

#include "face-manager.hpp"
#include "forwarder.hpp"
#include "tcp-server.hpp"
#include "udp-server.hpp"

namespace fcopss {
namespace rendezvous {

class Rendezvous final : noncopyable
{
public:
  Rendezvous(const fcopss::Config& config);

  void
  run();

  void
  onSignal(const boost::system::error_code& error, int signal);

private:
  boost::asio::io_service m_ioService;
  boost::asio::signal_set m_signals;
  unique_ptr<Forwarder> m_forwarder;
  unique_ptr<FaceManager> m_faceManager;
  unique_ptr<TcpServer> m_tcpServer;
  unique_ptr<UdpServer> m_udpServer;
};

} // namespace rendezvous
} // namespace fcopss

#endif // _FCOPSS_RENDEZVOUS_RENDEZVOUS_HPP_

