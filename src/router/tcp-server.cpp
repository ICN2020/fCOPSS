/*
  tcp-server.cpp

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
#include "tcp-server.hpp"
#include "tcp-transport.hpp"
#include "face-manager.hpp"

#include <boost/bind.hpp>

#include <fcopss/log.hpp>

namespace placeholders = boost::asio::placeholders;

using reuse_address = boost::asio::socket_base::reuse_address;
using reuse_port = boost::asio::detail::socket_option::boolean<SOL_SOCKET, SO_REUSEPORT>;

namespace fcopss {
namespace router {

TcpServer::TcpServer(io_service& ioService, uint16_t port, time_duration receiveTimeout, FaceManager& faceManager)
  : m_ioService(ioService),
    m_endPoint(tcp::v4(), port),
    m_acceptor(m_ioService),
    m_socket(m_ioService),
    m_receiveTimeout(receiveTimeout),
    m_faceManager(faceManager)
{
  m_acceptor.open(m_endPoint.protocol());
  m_acceptor.set_option(reuse_address(true));
  m_acceptor.set_option(reuse_port(true));
  m_acceptor.bind(m_endPoint);
  m_acceptor.listen();
}

void
TcpServer::start()
{
  asyncAccept();
}

void
TcpServer::asyncAccept()
{
  m_acceptor.async_accept(m_socket, boost::bind(&TcpServer::handleAccept, this, placeholders::error));
}

void
TcpServer::handleAccept(const boost::system::error_code& error)
{
  if (!error) {
    shared_ptr<Transport> transport = TcpTransport::create(std::move(m_socket), m_receiveTimeout);
    m_faceManager.createFace(transport);
    asyncAccept();
  } else {
    ERROR("%s", error.message().c_str());
  }
}

} // namespace router
} // namespace fcopss
