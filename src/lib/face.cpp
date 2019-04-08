/*
  face.cpp

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
#include <fcopss/face.hpp>
#include <fcopss/transport.hpp>
#include <fcopss/tcp-transport.hpp>
#include <fcopss/udp-transport.hpp>
#include <fcopss/config.hpp>
#include <fcopss/log.hpp>

namespace fcopss {
namespace client {

Face::Face()
  : m_internalIoService(new io_service)
  , m_ioService(*m_internalIoService)
  , m_deadlineTimer(m_ioService)
  , m_isSubscribing(false)
{
  setup();
}

Face::Face(io_service& ioService)
  : m_ioService(ioService)
  , m_deadlineTimer(m_ioService)
  , m_isSubscribing(false)
{
  setup();
}

Face::~Face()
{
  shutdown();
}

void
Face::setup()
{
  INFO("Setting up Face");
  Config config;
  
  m_subSendInterval = config.clientSubSendInterval().total_seconds();
  string protocol = config.clientProtocol();

  if (protocol == "tcp")
  {
    INFO("Creating TCP transport");
    m_transport = TcpTransport::create(m_ioService);
    m_transport->connect();
  }
  else if (protocol == "udp")
  {
    INFO("Creating UDP transport");
    m_transport = UdpTransport::create(m_ioService);
  }
  else
  {
    ERROR("Invalid protocol configuration. Please choose [udp] or [tcp]");
    BOOST_THROW_EXCEPTION(Error("Invalid protocol configuration. Please choose [udp] or [tcp]"));
  }
}

void
Face::subscribe(const Sub& sub, const OnPubReceived& onPub)
{
  if (m_isSubscribing)  // cancel timer before restarting
  {
    INFO("Canceling subscribe timer...");
    m_deadlineTimer.cancel();
  }
  else
  {
    m_isSubscribing = true;
    m_transport->registerCallback(onPub);  
    m_transport->asyncRead();    // call read only on the first run
  }

  if (sub.wireEncode().size() > MAX_PACKET_SIZE)
  {
    ERROR("Sub message size (%ld bytes) exceeds maximum limit (%ld bytes)", sub.wireEncode().size(), MAX_PACKET_SIZE);
    BOOST_THROW_EXCEPTION(Error("Sub message size exceeds maximum limit"));
  }
  
  m_transport->asyncWrite(sub);
  
  if (m_subSendInterval != 0)
  {
    DEBUG("Setting next subscribe timer to %ld seconds", m_subSendInterval);
    m_deadlineTimer.expires_from_now(boost::posix_time::seconds(m_subSendInterval));
    m_deadlineTimer.async_wait(bind(&Face::resendTimerHandler, this, std::placeholders::_1, sub));
  }
}

void
Face::resendTimerHandler(const boost::system::error_code& error, const Sub& sub)
{
  if (error)
  {
    if (error == boost::system::errc::operation_canceled) // operation explicitly cancelled
    {
      WARN("async_wait operation explicitly canceled");
      return;
    }
    
    shutdown();
    ERROR("Error in async_wait for resending next Sub packet");
    BOOST_THROW_EXCEPTION(Error(error, "Error in async_wait for resending next Sub packet"));
  }

  INFO("Resending sub packet");
  m_transport->asyncWrite(sub);
  
  INFO("Setting next subscribe timer");
  DEBUG("Setting next subscribe timer to %ld seconds", m_subSendInterval);
  m_deadlineTimer.expires_at(m_deadlineTimer.expires_at() + boost::posix_time::seconds(m_subSendInterval));
  m_deadlineTimer.async_wait(bind(&Face::resendTimerHandler, this, std::placeholders::_1, sub));
}

void
Face::publish(const PubToRp& pub)
{
  if (pub.wireEncode().size() > MAX_PACKET_SIZE)
  {
    ERROR("Pub message size (%ld bytes) exceeds maximum limit (%ld bytes)", pub.wireEncode().size(), MAX_PACKET_SIZE);
    BOOST_THROW_EXCEPTION(Error("Sub message size exceeds maximum limit"));
  }
  
  m_transport->asyncWrite(pub);
}

void
Face::run()
{
  INFO("Running io_service");
  m_ioService.run();
}

void
Face::shutdown()
{
  INFO("Shutting down face...");
  
  if (m_isSubscribing)  // cancel timer
  {
    INFO("Canceling subscribe timer...");
    m_deadlineTimer.cancel();
  }

  m_transport->close();
}

}   // namespace client
}   // namespace fcopss
