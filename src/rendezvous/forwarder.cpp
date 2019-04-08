/*
  forwarder.cpp

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
#include "forwarder.hpp"
#include "face-manager.hpp"

#include <fcopss/log.hpp>

namespace fcopss {
namespace rendezvous {

Forwarder::Forwarder(io_service& ioService, FaceManager& faceManager)
  : m_ioService(ioService), m_faceManager(faceManager)
{
  m_faceManager.setEventHandler(
    std::bind(&Forwarder::onSubReceived, this, _1, _2),
    std::bind(&Forwarder::onPubToRpReceived, this, _1, _2),
    std::bind(&Forwarder::onPubFromRpReceived, this, _1, _2),
    std::bind(&Forwarder::onFaceShutdown, this, _1)
  );
}

void
Forwarder::onSubReceived(const FaceId& faceId, const Sub& sub)
{
  INFO("Sub packet received from FaceID=%llu CD=%s", faceId, sub.getCd().toUri().c_str());
  // nothing to do
}

void
Forwarder::onPubToRpReceived(const FaceId& faceId, const PubToRp& pub)
{
  INFO("Pub to RP packet received from FaceID=%llu CD=%s", faceId, pub.getCd().toUri().c_str());
  bool doneForward = false;

  PubFromRp pubFromRp(pub);

  shared_ptr<Face> face = m_faceManager.find(faceId);
  if (face) {
    face->send(pubFromRp);
    INFO("Pub from RP packet CD=%s send to FaceID=%llu", pubFromRp.getCd().toUri().c_str(), face->getId());
    doneForward = true;
  }
 
  if (!doneForward) {
    WARN("Pub from RP packet CD=%s NOT send", pubFromRp.getCd().toUri().c_str());
  }
}

void
Forwarder::onPubFromRpReceived(const FaceId& faceId, const PubFromRp& pub)
{
  WARN("Pub from RP packet received from FaceID=%llu CD=%s", faceId, pub.getCd().toUri().c_str());
  // nothing to do
}

void
Forwarder::onFaceShutdown(FaceId faceId)
{
  INFO("FaceID=%llu shutdown, removing from tables", faceId);

  m_faceManager.remove(faceId);
}

} // namespace rendezvous
} // namespace fcopss

