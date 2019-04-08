/*
  face-manager.cpp

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
#include "face-manager.hpp"

#include <fcopss/log.hpp>

using namespace std::placeholders;

namespace fcopss {
namespace rendezvous {

FaceManager::FaceManager(io_service& ioService)
  : m_ioService(ioService), m_faceSerial(100)
{
}

void
FaceManager::setEventHandler(
  const SubReceivedCallback& onSubReceived,
  const PubToRpReceivedCallback& onPubToRpReceived,
  const PubFromRpReceivedCallback& onPubFromRpReceived,
  const FaceShutdownCallback& onFaceShutdown)
{
  m_onSubReceived = onSubReceived;
  m_onPubToRpReceived = onPubToRpReceived;
  m_onPubFromRpReceived = onPubFromRpReceived;
  m_onFaceShutdown = onFaceShutdown;
}

shared_ptr<Face>
FaceManager::createFace(const shared_ptr<Transport>& transport)
{
  FaceId id = createFaceId();
  shared_ptr<Face> face = Face::create(
                            id,
                            transport,
                            m_onSubReceived,
                            m_onPubToRpReceived,
                            m_onPubFromRpReceived,
                            m_onFaceShutdown,
                            m_ioService);

  transport->attach(face);

  m_faceTable[id] = face;

  INFO("FaceID=%llu Type=%s Local=%s:%hu Remote=%s:%hu added",
       face->getId(), face->getType().c_str(),
       face->getLocalIp().c_str(), face->getLocalPort(),
       face->getRemoteIp().c_str(), face->getRemotePort());

  transport->start();

  return face;
}

void
FaceManager::remove(const FaceId& faceId)
{
  m_faceTable.erase(faceId);

  INFO("FaceID=%llu removed", faceId);
}

shared_ptr<Face>
FaceManager::find(const FaceId& id) const
{
  shared_ptr<Face> face;

  FaceTable::const_iterator it = m_faceTable.find(id);
  if (it != m_faceTable.end()) {
    face = it->second;
  }

  return face;
}

shared_ptr<Face>
FaceManager::find(const string& type, const string& remoteIp, uint16_t remotePort) const
{
  shared_ptr<Face> face;

  for (FaceTable::const_iterator it = m_faceTable.begin(); it != m_faceTable.end(); it++) {
    if ((it->second->getType() == type) &&
        (it->second->getRemoteIp() == remoteIp) &&
        (it->second->getRemotePort() == remotePort)) {
      face = it->second;
      break;
    }
  }

  return face;
}

void
FaceManager::dump(vector<string>& lines) const
{
  lines.clear();
  for (auto it = m_faceTable.begin(); it != m_faceTable.end(); it++) {
    lines.push_back(it->second->toString());
  }
}

FaceId
FaceManager::createFaceId()
{
  m_faceSerial++;
  return m_faceSerial;
}

} // namespace rendezvous
} // namespace fcopss
