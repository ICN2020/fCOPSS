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
#include "fib.hpp"
#include "st.hpp"

#include <fcopss/log.hpp>

namespace fcopss {
namespace router {

Forwarder::Forwarder(io_service& ioService, Fib& fib, St& st, FaceManager& faceManager)
  : m_ioService(ioService), m_fib(fib), m_st(st), m_faceManager(faceManager)
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
  INFO("Sub packet received from FaceID=%lld CD=%s", faceId, sub.getCd().toUri().c_str());
  bool doneForward = false;

  bool doForward;
  Cd cd;
  tie(doForward, cd) = m_st.add(sub.getCd(), faceId);
  if (doForward) {
    Sub subToForward(cd);
    set<FaceId> outFaceIds = m_fib.match(cd);
    for (auto id = outFaceIds.begin(); id != outFaceIds.end(); id++) {
      shared_ptr<Face> face = m_faceManager.find(*id);
      if (face) {
        face->send(subToForward);
        INFO("Sub packet CD=%s forwarding to FaceID=%llu", sub.getCd().toUri().c_str(), face->getId());
        doneForward = true;
      }
    }
  }

  if (!doneForward) {
    INFO("Sub packet CD=%s NOT forwarding", sub.getCd().toUri().c_str());
  }
}

void
Forwarder::onPubToRpReceived(const FaceId& faceId, const PubToRp& pub)
{
  INFO("Pub to RP packet received from FaceID=%lld CD=%s", faceId, pub.getCd().toUri().c_str());
  bool doneForward = false;

  set<FaceId> outFaceIds = m_fib.match(pub.getCd());
  for (auto id = outFaceIds.begin(); id != outFaceIds.end(); id++) {
    shared_ptr<Face> face = m_faceManager.find(*id);
    if (face) {
      face->send(pub);
      INFO("Pub to RP packet CD=%s forwarding to FaceID=%llu", pub.getCd().toUri().c_str(), face->getId());
      doneForward = true;
    }
  }

  if (!doneForward) {
    INFO("Pub to RP packet CD=%s NOT forwarding", pub.getCd().toUri().c_str());
  }
}

void
Forwarder::onPubFromRpReceived(const FaceId& faceId, const PubFromRp& pub)
{
  INFO("Pub from RP packet received from FaceID=%lld CD=%s", faceId, pub.getCd().toUri().c_str());
  bool doneForward = false;

  set<FaceId> outFaceIds = m_st.match(pub.getCd());
  for (auto id = outFaceIds.begin(); id != outFaceIds.end(); id++) {
    shared_ptr<Face> face = m_faceManager.find(*id);
    if (face) {
      face->send(pub);
      INFO("Pub from RP packet CD=%s forwarding to FaceID=%llu", pub.getCd().toUri().c_str(), face->getId());
      doneForward = true;
    }
  }

  if (!doneForward) {
    INFO("Pub from RP packet CD=%s NOT forwarding", pub.getCd().toUri().c_str());
  }
}

void
Forwarder::onFaceShutdown(FaceId faceId)
{
  INFO("FaceID=%llu shutdown, removing from tables", faceId);

  m_st.remove(faceId);
  m_fib.remove(faceId);
  m_faceManager.remove(faceId);
}

} // namespace router
} // namespace fcopss

