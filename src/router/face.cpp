/*
  face.cpp

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
#include "face.hpp"

#include <fcopss/log.hpp>

namespace fcopss {
namespace router {

shared_ptr<Face>
Face::create(
    FaceId id,
    const shared_ptr<Transport>& transport,
    const SubReceivedCallback& onSubReceived,
    const PubToRpReceivedCallback& onPubToRpReceived,
    const PubFromRpReceivedCallback& onPubFromRpReceived,
    const FaceShutdownCallback& onFaceShutdown,
    boost::asio::io_service& ioService)
{
  return shared_ptr<Face>(new Face(id, transport, onSubReceived, onPubToRpReceived, onPubFromRpReceived, onFaceShutdown, ioService));
}

Face::Face(
    FaceId id,
    const shared_ptr<Transport>& transport,
    const SubReceivedCallback& onSubReceived,
    const PubToRpReceivedCallback& onPubToRpReceived,
    const PubFromRpReceivedCallback& onPubFromRpReceived,
    const FaceShutdownCallback& onFaceShutdown,
    boost::asio::io_service& ioService)
      : m_id(id),
        m_transport(transport),
        m_onSubReceived(onSubReceived),
        m_onPubToRpReceived(onPubToRpReceived),
        m_onPubFromRpReceived(onPubFromRpReceived),
        m_onFaceShutdown(onFaceShutdown),
        m_ioService(ioService)
{
  m_type = transport->getType();
  m_localIp = transport->getLocalIp();
  m_localPort = transport->getLocalPort(); 
  m_remoteIp = transport->getRemoteIp();
  m_remotePort = transport->getRemotePort();
}

void
Face::onPacketReceived(Block&& packet)
{
  try {
    if (packet.type() == tlv::Sub) {
      auto sub = make_shared<Sub>(packet);
      m_onSubReceived(m_id, *sub);
    } else if (packet.type() == tlv::PubToRp) {
      auto pub = make_shared<PubToRp>(packet);
      m_onPubToRpReceived(m_id, *pub);
    } else if (packet.type() == tlv::PubFromRp) {
      auto pub = make_shared<PubFromRp>(packet);
      m_onPubFromRpReceived(m_id, *pub);
    } else {
      WARN("unknown packet received: TLV type=%d", packet.type());
    }
  } catch (const runtime_error& e) {
    ERROR("%s", e.what());
  }
}

void
Face::onTransportShutdown()
{
  m_ioService.post(std::bind(m_onFaceShutdown, m_id));
}

void
Face::shutdown()
{
  m_transport->shutdown();
}

void
Face::send(const Sub& sub)
{
  Block packet(sub.wireEncode());
  m_transport->send(std::move(packet));
}

void
Face::send(const PubToRp& pub)
{
  Block packet(pub.wireEncode());
  m_transport->send(std::move(packet));
}

void
Face::send(const PubFromRp& pub)
{
  Block packet(pub.wireEncode());
  m_transport->send(std::move(packet));
}

const FaceId&
Face::getId() const
{
  return m_id;
}

const string&
Face::getType() const
{
  return m_type;
}

const string&
Face::getLocalIp() const
{
  return m_localIp;
}

uint16_t
Face::getLocalPort() const
{
  return m_localPort;
}

const string&
Face::getRemoteIp() const
{
  return m_remoteIp;
}

uint16_t
Face::getRemotePort() const
{
  return m_remotePort;
}

string
Face::toString() const
{
  stringstream ss;

  ss << m_id;
  ss << "=(";
  ss << m_type;
  ss << ",";
  ss << m_localIp << ":" << m_localPort;
  ss << ",";
  ss << m_remoteIp << ":" << m_remotePort;
  ss << ")";

  return ss.str();
}

} // namespace router
} // namespace fcopss
