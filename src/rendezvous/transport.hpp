/*
  transport.hpp

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
#ifndef _FCOPSS_RENDEZVOUS_TRANSPORT_HPP_
#define _FCOPSS_RENDEZVOUS_TRANSPORT_HPP_

#include <fcopss/common.hpp>

namespace fcopss {
namespace rendezvous {

class Face;

class Transport : noncopyable
{
public:
  Transport();

  virtual
  ~Transport();

  void
  attach(const shared_ptr<Face>& face);

  virtual void
  start() = 0;

  virtual void
  send(Block&& packet) = 0;

  virtual string
  getType() const = 0;
  
  virtual string
  getLocalIp() const = 0;

  virtual uint16_t
  getLocalPort() const = 0;

  virtual string
  getRemoteIp() const = 0;

  virtual uint16_t
  getRemotePort() const = 0;

  virtual void
  shutdown() = 0;

protected:
  weak_ptr<Face> m_face;
};

} // namespace rendezvous
} // namespace fcopss

#endif // _FCOPSS_RENDEZVOUS_TRANSPORT_HPP_
