/*
  face.hpp

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
#ifndef _FCOPSS_ROUTER_FACE_HPP_
#define _FCOPSS_ROUTER_FACE_HPP_

#include <fcopss/common.hpp>
#include <fcopss/sub.hpp>
#include <fcopss/pub-to-rp.hpp>
#include <fcopss/pub-from-rp.hpp>

#include "transport.hpp"

namespace fcopss {
namespace router {

using FaceId = uint64_t;
using SubReceivedCallback = function<void(const FaceId&, const Sub&)>;
using PubToRpReceivedCallback = function<void(const FaceId&, const PubToRp&)>;
using PubFromRpReceivedCallback = function<void(const FaceId&, const PubFromRp&)>; 
using FaceShutdownCallback = function<void(FaceId)>; 

class Face final : public noncopyable
{
public:
  static shared_ptr<Face>
  create(
    FaceId id,
    const shared_ptr<Transport>& transport, 
    const SubReceivedCallback& onSubReceived,
    const PubToRpReceivedCallback& onPubToRpReceived,
    const PubFromRpReceivedCallback& onPubFromRpReceived,
    const FaceShutdownCallback& onFaceShutdown,
    boost::asio::io_service& ioService
  );

  void
  onPacketReceived(Block&& packet);

  void
  onTransportShutdown();

  void
  shutdown();

  void
  send(const Sub& sub);

  void
  send(const PubToRp& pub);

  void
  send(const PubFromRp& pub);

  const FaceId&
  getId() const;

  const string&
  getType() const;

  const string&
  getLocalIp() const;

  uint16_t
  getLocalPort() const;

  const string&
  getRemoteIp() const;

  uint16_t
  getRemotePort() const;

  string
  toString() const;

private:
  Face(
    FaceId id,
    const shared_ptr<Transport>& transport, 
    const SubReceivedCallback& onSubReceived,
    const PubToRpReceivedCallback& onPubToRpReceived,
    const PubFromRpReceivedCallback& onPubFromRpReceived,
    const FaceShutdownCallback& onFaceShutdown,
    boost::asio::io_service& ioService
  );

private:
  FaceId m_id;
  shared_ptr<Transport> m_transport;
  SubReceivedCallback m_onSubReceived;
  PubToRpReceivedCallback m_onPubToRpReceived;
  PubFromRpReceivedCallback m_onPubFromRpReceived;
  FaceShutdownCallback m_onFaceShutdown;
  string m_type;
  string m_localIp;
  uint16_t m_localPort;
  string m_remoteIp;
  uint16_t m_remotePort;
  boost::asio::io_service& m_ioService;
};

} // namespace router
} // namespace fcopss

#endif // _FCOPSS_ROUTER_FACE_HPP_
