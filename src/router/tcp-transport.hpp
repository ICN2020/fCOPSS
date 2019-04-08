/*
  tcp-transport.hpp

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
#ifndef _FCOPSS_ROUTER_TCP_TRANSPORT_HPP_
#define _FCOPSS_ROUTER_TCP_TRANSPORT_HPP_

#include <queue>

#include "transport.hpp"

namespace fcopss {
namespace router {

class TcpTransport final : public Transport, public enable_shared_from_this<TcpTransport>
{
public:
  static shared_ptr<TcpTransport>
  create(tcp::socket&& s, const time_duration& receiveTimeout);

  ~TcpTransport();

  void
  start() override;

  void
  send(Block&& packet) override;

  string
  getType() const override;

  string
  getLocalIp() const override;

  uint16_t
  getLocalPort() const override;

  string
  getRemoteIp() const override;

  uint16_t
  getRemotePort() const override;

  void
  shutdown() override;

private:
  TcpTransport(tcp::socket&& s, const time_duration& receiveTimeout);

  void
  handleShutdown();

  void
  asyncReceive(const time_duration& timeout);

  void
  handleReceive(const boost::system::error_code& error, size_t bytes_transferred);

  void
  handleTimeout(const boost::system::error_code& error);

  void
  asyncSend();

  void
  handleSend(const boost::system::error_code& error, size_t bytes_transferred);

private:
  tcp::socket m_socket;
  uint8_t m_receiveBuffer[MAX_PACKET_SIZE];
  size_t m_receivedBufferSize;
  time_duration m_receiveTimeout;
  deadline_timer m_receiveTimer;
  std::queue<Block> m_sendQueue;
};

} // namespace router
} // namespace fcopss

#endif // _FCOPSS_ROUTER_TCP_TRANSPORT_HPP_

