/*
  udp-transport.hpp

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
#ifndef _FCOPSS_RENDEZVOUS_UDP_TRANSPORT_HPP_
#define _FCOPSS_RENDEZVOUS_UDP_TRANSPORT_HPP_

#include "transport.hpp"

namespace fcopss {
namespace rendezvous {

class UdpTransport final : public Transport, public enable_shared_from_this<UdpTransport>
{
public:
  static shared_ptr<UdpTransport>
  create(udp::socket&& s, const uint8_t* receiveBuffer, size_t receivedBufferSize, const boost::system::error_code& error);

  static shared_ptr<UdpTransport>
  create(udp::socket&& s);

  ~UdpTransport();

  void
  start() override;

  void
  send(Block&& packe) override;

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
  UdpTransport(udp::socket&& s, const uint8_t* receiveBuffer, size_t receivedBufferSize, const boost::system::error_code& error);

  UdpTransport(udp::socket&& s);

  void
  handleShutdown();

  void
  asyncReceive();

  void
  handleReceive(const boost::system::error_code& error, size_t bytes_transferred);

  void
  handleReceive();

  void
  handleSend(const boost::system::error_code& error, size_t bytes_transferred);

private:
  uint8_t m_receiveBuffer[MAX_PACKET_SIZE];
  size_t m_receivedBufferSize;
  boost::system::error_code m_receiveError;
  udp::socket m_socket;
};

} // namespace rendezvous
} // namespace fcopss

#endif // _FCOPSS_RENDEZVOUS_UDP_TRANSPORT_HPP_

