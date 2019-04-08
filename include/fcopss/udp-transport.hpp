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
#ifndef _FCOPSS_UDP_TRANSPORT_HPP_
#define _FCOPSS_UDP_TRANSPORT_HPP_

#include <fcopss/transport.hpp>

namespace fcopss {
namespace client {

class UdpTransport :public Transport, public enable_shared_from_this<UdpTransport>
{
public:
  UdpTransport(io_service& ioService) ;

  virtual
  ~UdpTransport() override;

  virtual void
  asyncWrite(const PubToRp& pub) override;

  virtual void
  asyncWrite(const Sub& sub) override;

  virtual void
  asyncRead() override;
 
  static shared_ptr<UdpTransport>
  create(io_service& ioSevice);

  virtual void
  close() override;

private:
  udp::socket   m_socket;
  udp::endpoint m_endpoint;
  string        m_routerPort;

  void 
  getEndPoints();

};  // class UdpTransport

}   // namespace client
}   // namespace fcopss


#endif
