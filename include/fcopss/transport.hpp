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
#ifndef _FCOPSS_TRANSPORT_HPP_
#define _FCOPSS_TRANSPORT_HPP_

#include <fcopss/common.hpp>
#include <fcopss/sub.hpp>
#include <fcopss/pub-from-rp.hpp>
#include <fcopss/pub-to-rp.hpp>

namespace fcopss {
namespace client {

typedef function<void(const PubFromRp&, const boost::system::error_code&)> OnPubReceived;

class Transport : noncopyable
{
public:
  class Error : public std::runtime_error
  {
  public:
    Error(const boost::system::error_code& code, const string& msg)
      : std::runtime_error(msg + (code.value() ? (" (" + code.category().message(code.value()) + ")") : ("")))
      {
      }

    explicit
    Error(const string& what)
      : std::runtime_error(what)
    {
    }
  };

  Transport(io_service& ioService)
    : m_ioService(ioService)
  {
  }

  virtual
  ~Transport() = default;

  virtual  void
  asyncWrite(const PubToRp& pub) = 0;

  virtual void
  asyncWrite(const Sub& sub) = 0;

  virtual void
  asyncRead() = 0;

  virtual void
  close() = 0;

  virtual void
  connect() {}

  void
  registerCallback(const OnPubReceived& onPub)
  {
    m_onPubReceived = onPub;
  }

protected:
  io_service&   m_ioService;
  uint8_t       m_inputBuffer[MAX_PACKET_SIZE];
  OnPubReceived m_onPubReceived;

  void
  recvCallback(const PubFromRp& pubFromRp, const boost::system::error_code& error);
};

inline void
Transport::recvCallback(const PubFromRp& pubFromRp, const boost::system::error_code& error)
{
  m_onPubReceived(pubFromRp, error);
}

}   // namespace fcopss
}   // namespace client

#endif
