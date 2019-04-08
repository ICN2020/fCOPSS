/*
  udp-transport.cpp

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
#include "udp-transport.hpp"
#include "face.hpp"

#include <boost/bind.hpp>

#include <fcopss/log.hpp>

namespace placeholders = boost::asio::placeholders;

namespace fcopss {
namespace router {

shared_ptr<UdpTransport>
UdpTransport::create(udp::socket&& s, const uint8_t* receiveBuffer, size_t receivedBufferSize, const boost::system::error_code& error)
{
  return shared_ptr<UdpTransport>(new UdpTransport(std::move(s), receiveBuffer, receivedBufferSize, error));
}

shared_ptr<UdpTransport>
UdpTransport::create(udp::socket&& s)
{
  return shared_ptr<UdpTransport>(new UdpTransport(std::move(s)));
}

UdpTransport::UdpTransport(udp::socket&& s, const uint8_t* receiveBuffer, size_t receivedBufferSize, const boost::system::error_code& error)
  : m_receivedBufferSize(receivedBufferSize),
    m_receiveError(error),
    m_socket(std::move(s))
{
  memcpy(m_receiveBuffer, receiveBuffer, receivedBufferSize);
}

UdpTransport::UdpTransport(udp::socket&& s)
  : m_receivedBufferSize(0),
    m_socket(std::move(s))
{
}

UdpTransport::~UdpTransport()
{
}

void
UdpTransport::start()
{
  handleReceive();  
}

void
UdpTransport::send(Block&& packet)
{
  m_socket.async_send(
    boost::asio::buffer(packet),
    boost::bind(&UdpTransport::handleSend, shared_from_this(), placeholders::error, placeholders::bytes_transferred)
  );
}

string
UdpTransport::getType() const
{
  return string("udp");
}

string
UdpTransport::getLocalIp() const
{
  udp::endpoint local = m_socket.local_endpoint();

  return local.address().to_string();
}

uint16_t
UdpTransport::getLocalPort() const
{
  udp::endpoint local = m_socket.local_endpoint();

  return local.port();
}

string
UdpTransport::getRemoteIp() const
{
  udp::endpoint remote = m_socket.remote_endpoint();

  return remote.address().to_string();
}

uint16_t
UdpTransport::getRemotePort() const
{
  udp::endpoint remote = m_socket.remote_endpoint();

  return remote.port();
}

void
UdpTransport::shutdown()
{
  boost::system::error_code ec;
  m_socket.cancel(ec);

  m_socket.get_io_service().post(boost::bind(&UdpTransport::handleShutdown, shared_from_this()));
}

void
UdpTransport::handleShutdown()
{
  boost::system::error_code ec;
  m_socket.close(ec);

  auto face = m_face.lock();
  if (face) {
    face->onTransportShutdown();
  }
}

void
UdpTransport::asyncReceive()
{
  m_socket.async_receive(
    boost::asio::buffer(m_receiveBuffer, sizeof (m_receiveBuffer)),
    boost::bind(&UdpTransport::handleReceive, shared_from_this(), placeholders::error, placeholders::bytes_transferred)
  );
}

void
UdpTransport::handleReceive(const boost::system::error_code& error, size_t bytes_transferred)
{
  m_receivedBufferSize = bytes_transferred;
  m_receiveError = error;
  handleReceive();
}

void
UdpTransport::handleReceive()
{
  if (!m_receiveError) {
    if (m_receivedBufferSize == 0) {
      asyncReceive();
    } else {
      bool ok = false;
      Block block;
      tie(ok, block) = Block::fromBuffer(m_receiveBuffer, m_receivedBufferSize);
      if (!ok) {
        ERROR("packet parse error");
        shutdown();
        return;
      }
      if (block.size() < m_receivedBufferSize) {
        ERROR("packet has %lu bytes trailer", m_receivedBufferSize - block.size());
        shutdown();
        return;
      }
      auto face = m_face.lock();
      if (face) {
        face->onPacketReceived(std::move(block));
      }
      asyncReceive();
    }
  } else if (m_receiveError == boost::asio::error::operation_aborted) {
    DEBUG("%s", m_receiveError.message().c_str());
  } else {
    ERROR("%s", m_receiveError.message().c_str());
    shutdown();
  }
}

void
UdpTransport::handleSend(const boost::system::error_code& error, size_t bytes_transferred)
{
  if (error) {
    ERROR("%s", error.message().c_str());
    shutdown();
  }
}

} // namespace router
} // namespace fcopss

