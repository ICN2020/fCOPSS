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
#ifndef _FCOPSS_TCP_TRANSPORT_HPP_
#define _FCOPSS_TCP_TRANSPORT_HPP_

#include <fcopss/transport.hpp>

namespace fcopss {
namespace client {

typedef std::list<Block> TransmissionQueue;

class TcpTransport :public Transport, public enable_shared_from_this<TcpTransport>
{
public:
  TcpTransport(io_service& ioService) ;

  virtual
  ~TcpTransport() override;

  virtual void
  asyncWrite(const PubToRp& pub) override;

  virtual void
  asyncWrite(const Sub& sub) override;

  virtual void
  asyncRead() override;

  static shared_ptr<TcpTransport>
  create(io_service& ioSevice);

  virtual void
  close() override;

  virtual void
  connect() override;

private:
  tcp::socket       m_socket;
  deadline_timer    m_deadlineTimer;
  bool              m_isTimerStarted;
  long              m_tcpReceiveTimeout;
  long              m_tcpConnectionTimeout;
  string            m_routerPort;
  size_t            m_inputBufferSize;
  TransmissionQueue m_transmissionQueue;
  int               m_isConnected;  // -1: not connected, 0: connecting, 1: connected
  bool              m_isReadFromConnectHandler;
  tcp::resolver     m_resolver;

  void
  connectTimeoutHandler(const boost::system::error_code& error);

  void
  resolveHandler(const boost::system::error_code& error, tcp::resolver::iterator i);

  void 
  connectHandler(const boost::system::error_code& error, tcp::resolver::iterator i);

  void
  ensureConnected();

  void
  send();

  void
  receive();

  bool
  processMessage(size_t& offset);

};  // class TcpTransport

}   // namespace client
}   // namespace fcopss


#endif
