/*
  router.cpp

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
#include "router.hpp"
#include "fib.hpp"
#include "st-impl.hpp"
#include "forwarder.hpp"
#include "face-manager.hpp"
#include "tcp-server.hpp"
#include "udp-server.hpp"
#include "cmd-server.hpp"

#include <fcopss/log.hpp>

namespace placeholders = boost::asio::placeholders;

namespace fcopss {
namespace router {

Router::Router(const fcopss::Config& config)
  : m_signals(m_ioService)
{
  m_signals.add(SIGTERM);
  m_signals.add(SIGINT);

  m_fib.reset(new Fib());

  m_st.reset(new StImpl(m_ioService, config.routerStExpireTime()));

  m_faceManager.reset(new FaceManager(m_ioService));

  m_forwarder.reset(new Forwarder(m_ioService, *m_fib, *m_st, *m_faceManager));

  m_tcpServer.reset(
    new TcpServer(m_ioService, config.routerPort(), config.tcpReceiveTimeout(), *m_faceManager)
  );

  m_udpServer.reset(new UdpServer(m_ioService, config.routerPort(), *m_faceManager));

  m_cmdServer.reset(
    new CmdServer(
      m_ioService,
      config.routerControlPort(),
      config.routerPort(),
      config.tcpReceiveTimeout(),
      config.tcpConnectionTimeout(),
      *m_faceManager,
      *m_fib,
      *m_st
    )
  );  
}

void
Router::run()
{
  m_signals.async_wait(boost::bind(&Router::onSignal, this, placeholders::error, placeholders::signal_number));

  m_tcpServer->start();
  m_udpServer->start();
  m_cmdServer->start();

  INFO("router start");
  m_ioService.run();
  INFO("router end");
}

void
Router::onSignal(const boost::system::error_code& error, int signal)
{
  m_ioService.stop();
}

} // namespace fcopss
} // namespace router
