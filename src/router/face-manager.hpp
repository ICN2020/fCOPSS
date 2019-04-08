/*
  face-manager.hpp

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
#ifndef _FCOPSS_ROUTER_FACE_MANAGER_HPP_
#define _FCOPSS_ROUTER_FACE_MANAGER_HPP_

#include <map>

#include <fcopss/common.hpp>

#include "face.hpp"

namespace fcopss {
namespace router {

using FaceTable = std::map<FaceId, shared_ptr<Face>>;

class FaceManager final : noncopyable
{
public:
  FaceManager(io_service& ioService);

  void
  setEventHandler(
    const SubReceivedCallback& onSubReceived,
    const PubToRpReceivedCallback& onPubToRpReceived,
    const PubFromRpReceivedCallback& onPubFromRpReceived,
    const FaceShutdownCallback& onFaceShutdown
  );

  shared_ptr<Face>
  createFace(const shared_ptr<Transport>& transport);

  void
  remove(const FaceId& faceId);

  shared_ptr<Face>
  find(const FaceId& id) const;

  shared_ptr<Face>
  find(const string& type, const string& remoteIp, uint16_t remotePort) const;

  void
  dump(vector<string>& lines) const;

private:
  FaceId
  createFaceId();

private:
  io_service& m_ioService;

  SubReceivedCallback m_onSubReceived;
  PubToRpReceivedCallback m_onPubToRpReceived;
  PubFromRpReceivedCallback m_onPubFromRpReceived;
  FaceShutdownCallback m_onFaceShutdown;

  FaceId m_faceSerial;
  FaceTable m_faceTable;
};

} // namespace router
} // namespace fcopss

#endif // _FCOPSS_ROUTER_FACE_MANAGER_HPP_
