/*
  tcp-server.hpp

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
#ifndef _FCOPSS_RENDEZVOUS_TCP_SERVER_HPP_
#define _FCOPSS_RENDEZVOUS_TCP_SERVER_HPP_

#include <fcopss/common.hpp>

namespace fcopss {
namespace rendezvous {

class FaceManager;

class TcpServer final : public noncopyable
{
public:
  TcpServer(io_service& ioService, uint16_t port, time_duration receiveTimeout, FaceManager& faceManager);

  void
  start();

  void
  asyncAccept();

  void
  handleAccept(const boost::system::error_code& error);

private:
  io_service& m_ioService;
  tcp::endpoint m_endPoint;
  tcp::acceptor m_acceptor;
  tcp::socket m_socket;
  time_duration m_receiveTimeout;
  FaceManager& m_faceManager;
};

} // namespace rendezvous
} // namespace fcopss

#endif // _FCOPSS_RENDEZVOUS_TCP_SERVER_HPP_

