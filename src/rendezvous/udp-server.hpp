/*
  udp-server.hpp

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
#ifndef _FCOPSS_RENDEZVOUS_UDP_SERVER_HPP_
#define _FCOPSS_RENDEZVOUS_UDP_SERVER_HPP_

#include <fcopss/common.hpp>

namespace fcopss {
namespace rendezvous {

class FaceManager;

class UdpServer final : public noncopyable
{
public:
  UdpServer(io_service& ioService, uint16_t port, FaceManager& faceManager);

  void
  start();

  void
  asyncReceive();

  void
  handleReceive(const boost::system::error_code& error, size_t bytes_transferred);

private:
  io_service& m_ioService;
  udp::endpoint m_localEndPoint;
  udp::endpoint m_remoteEndPoint;
  udp::socket m_socket;
  uint8_t m_receiveBuffer[MAX_PACKET_SIZE];
  FaceManager& m_faceManager;
};

} // namespace rendezvous
} // namespace fcopss

#endif // _FCOPSS_RENDEZVOUS_UDP_SERVER_HPP_

