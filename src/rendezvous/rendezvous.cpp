/*
  rendezvous.cpp

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
#include "rendezvous.hpp"
#include "forwarder.hpp"
#include "face-manager.hpp"
#include "tcp-server.hpp"
#include "udp-server.hpp"

#include <fcopss/log.hpp>

namespace placeholders = boost::asio::placeholders;

namespace fcopss {
namespace rendezvous {

Rendezvous::Rendezvous(const fcopss::Config& config)
  : m_signals(m_ioService)
{
  m_signals.add(SIGTERM);
  m_signals.add(SIGINT);

  m_faceManager.reset(new FaceManager(m_ioService));

  m_forwarder.reset(new Forwarder(m_ioService, *m_faceManager));

  m_tcpServer.reset(
    new TcpServer(m_ioService, config.rpPort(), config.tcpReceiveTimeout(), *m_faceManager)
  );

  m_udpServer.reset(new UdpServer(m_ioService, config.rpPort(), *m_faceManager));
}

void
Rendezvous::run()
{
  m_signals.async_wait(boost::bind(&Rendezvous::onSignal, this, placeholders::error, placeholders::signal_number));

  m_tcpServer->start();
  m_udpServer->start();

  INFO("rendezvous start");
  m_ioService.run();
  INFO("rendezvous end");
}

void
Rendezvous::onSignal(const boost::system::error_code& error, int signal)
{
  m_ioService.stop();
}

} // namespace fcopss
} // namespace rendezvous
