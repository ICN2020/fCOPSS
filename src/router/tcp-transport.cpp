/*
  tcp-transport.cpp

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
#include "tcp-transport.hpp"
#include "face.hpp"

#include <boost/bind.hpp>

#include <fcopss/log.hpp>

namespace asio = boost::asio;
namespace placeholders = boost::asio::placeholders;

namespace fcopss {
namespace router {

shared_ptr<TcpTransport>
TcpTransport::create(tcp::socket&& s, const time_duration& receiveTimeout)
{
  return shared_ptr<TcpTransport>(new TcpTransport(std::move(s), receiveTimeout));
}

TcpTransport::TcpTransport(tcp::socket&& s, const time_duration& receiveTimeout)
  : m_socket(std::move(s)),
    m_receivedBufferSize(0),
    m_receiveTimeout(receiveTimeout),
    m_receiveTimer(m_socket.get_io_service())
{
}

TcpTransport::~TcpTransport()
{
}

void
TcpTransport::start()
{
  asyncReceive(time_duration(boost::posix_time::pos_infin));
}

void TcpTransport::send(Block&& packet)
{
  bool wasQueueEmpty = m_sendQueue.empty();

  m_sendQueue.push(std::move(packet));

  if (wasQueueEmpty) {
    asyncSend();
  }
}

string
TcpTransport::getType() const
{
  return string("tcp");
}

string
TcpTransport::getLocalIp() const
{
  tcp::endpoint local = m_socket.local_endpoint();

  return local.address().to_string();
}

uint16_t
TcpTransport::getLocalPort() const
{
  tcp::endpoint local = m_socket.local_endpoint();

  return local.port();
}

string
TcpTransport::getRemoteIp() const
{
  tcp::endpoint remote = m_socket.remote_endpoint();

  return remote.address().to_string();
}

uint16_t
TcpTransport::getRemotePort() const
{
  tcp::endpoint remote = m_socket.remote_endpoint();

  return remote.port();
}

void
TcpTransport::shutdown()
{
  boost::system::error_code ec;
  m_socket.shutdown(tcp::socket::shutdown_both, ec);
  m_socket.cancel(ec);

  m_socket.get_io_service().post(boost::bind(&TcpTransport::handleShutdown, shared_from_this()));
}

void
TcpTransport::handleShutdown()
{
  std::queue<Block>().swap(m_sendQueue);

  boost::system::error_code ec;
  m_socket.close(ec);

  auto face = m_face.lock();
  if (face) {
    face->onTransportShutdown();
  }
}

void
TcpTransport::asyncReceive(const time_duration& timeout)
{
  m_socket.async_receive(
    asio::buffer(m_receiveBuffer + m_receivedBufferSize, MAX_PACKET_SIZE - m_receivedBufferSize),
    boost::bind(&TcpTransport::handleReceive, shared_from_this(), placeholders::error, placeholders::bytes_transferred)
  );

  m_receiveTimer.expires_from_now(timeout);
  m_receiveTimer.async_wait(bind(&TcpTransport::handleTimeout, shared_from_this(), placeholders::error));
}

void
TcpTransport::handleReceive(const boost::system::error_code& error, size_t bytes_transferred)
{
  m_receiveTimer.cancel();

  if (!error) {
    m_receivedBufferSize += bytes_transferred;
    size_t offset = 0;

    bool ok = true;
    Block block;
    while ((m_receivedBufferSize - offset) > 0) {
      tie(ok, block) = Block::fromBuffer(m_receiveBuffer + offset, m_receivedBufferSize - offset);
      if (!ok) {
        break;
      }
      offset += block.size();
      auto face = m_face.lock();
      if (face) {
        face->onPacketReceived(std::move(block));
      }
    }

    if (!ok && (m_receivedBufferSize == sizeof (m_receiveBuffer)) && (offset == 0)) {
      ERROR("packet parse errort");
      shutdown();
      return;
    }

    time_duration timeout(boost::posix_time::pos_infin);

    if (offset > 0) {
      if (offset < m_receivedBufferSize) {
        std::copy(m_receiveBuffer + offset, m_receiveBuffer + m_receivedBufferSize, m_receiveBuffer);
        m_receivedBufferSize -= offset;
        timeout = m_receiveTimeout;
        if (!timeout.is_special()) {
          DEBUG("continuous read: receive timer set to %ld sec", timeout.total_seconds());
        }
      } else {
        m_receivedBufferSize = 0;
      }
    } else {
      timeout = m_receiveTimeout;
      if (!timeout.is_special()) {
        DEBUG("continuous read: receive timer set to %ld sec", timeout.total_seconds());
      }
    }

    asyncReceive(timeout);
  } else if (error == asio::error::operation_aborted) {
    DEBUG("%s", error.message().c_str());
  } else {
    if ((error == asio::error::eof) || (error == asio::error::connection_reset)) {
      DEBUG("%s", error.message().c_str());
    } else {
      ERROR("%s", error.message().c_str());
    }
    shutdown();
  }
}

void
TcpTransport::handleTimeout(const boost::system::error_code& error)
{
  if (!error) {
    auto face = m_face.lock();
    if (face) {
      WARN("receive timeout FaceID=%llu Remote=%s:%hu", face->getId(), getRemoteIp().c_str(), getRemotePort());
    } else {
      WARN("receive timeout Remote=%s:%hu", getRemoteIp().c_str(), getRemotePort());
    }
    shutdown();
  }
}

void
TcpTransport::asyncSend()
{
  asio::async_write(
    m_socket,
    asio::buffer(m_sendQueue.front()),
    boost::bind(&TcpTransport::handleSend, shared_from_this(), placeholders::error, placeholders::bytes_transferred) 
  );
}

void
TcpTransport::handleSend(const boost::system::error_code& error, size_t bytes_transferred)
{
  if (!error) {
    m_sendQueue.pop();
    if (!m_sendQueue.empty()) {
      asyncSend();
    }
  } else {
    ERROR("%s", error.message().c_str());
    shutdown();
  }
}

} // namespace router
} // namespace fcopss

