/*
  udp-server.cpp

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
#include "udp-server.hpp"
#include "udp-transport.hpp"
#include "face-manager.hpp"

#include <boost/bind.hpp>

#include <fcopss/log.hpp>

namespace placeholders = boost::asio::placeholders;

using reuse_address = boost::asio::socket_base::reuse_address;
using reuse_port = boost::asio::detail::socket_option::boolean<SOL_SOCKET, SO_REUSEPORT>;

namespace fcopss {
namespace router {

UdpServer::UdpServer(io_service& ioService, uint16_t port, FaceManager& faceManager)
  : m_ioService(ioService),
    m_localEndPoint(udp::v4(), port),
    m_socket(ioService),
    m_faceManager(faceManager)
{
}

void
UdpServer::start()
{
  m_socket.open(udp::v4());

  m_socket.set_option(reuse_address(true));
  m_socket.set_option(reuse_port(true));

  m_socket.bind(m_localEndPoint);

  asyncReceive();
}

void
UdpServer::asyncReceive()
{
  m_socket.async_receive_from(
    boost::asio::buffer(m_receiveBuffer, sizeof (m_receiveBuffer)),
    m_remoteEndPoint,
    bind(&UdpServer::handleReceive, this, placeholders::error, placeholders::bytes_transferred)
  );
}

void
UdpServer::handleReceive(const boost::system::error_code& error, size_t bytes_transferred)
{
  if (!error) {
    udp::socket s(m_ioService);
    s.open(udp::v4());
    s.set_option(reuse_address(true));
    s.set_option(reuse_port(true));
    s.bind(m_localEndPoint);
    s.connect(m_remoteEndPoint);

    shared_ptr<Transport> transport = UdpTransport::create(std::move(s), m_receiveBuffer, bytes_transferred, error);
    m_faceManager.createFace(transport);

  } else {
    ERROR("%s", error.message().c_str());
  }

  asyncReceive();
}

} // namespace router
} // namespace fcopss

