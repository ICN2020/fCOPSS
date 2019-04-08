/*
  fib.cpp

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
#include "fib.hpp"

#include <fcopss/log.hpp>

namespace fcopss {
namespace router {

void
Fib::add(const string& name, const FaceId& faceId, uint32_t cost)
{
  Cd cd(name);
  FibEntry::NextHop nextHop(faceId, cost);

  auto it = std::find_if(m_fib.begin(), m_fib.end(), [&](FibEntry& entry) { return entry.m_cd == cd; });
  if (it != m_fib.end()) {
    it->m_nextHops.erase(nextHop);
    it->m_nextHops.insert(nextHop);
    INFO("FIB entry FaceID=%llu CD=%s already exists, update entry", faceId, cd.toUri().c_str());
  } else {
    FibEntry entry(cd, nextHop);
    m_fib.push_back(entry);
    INFO("FIB entry FaceID=%llu CD=%s not exists, create entry", faceId, cd.toUri().c_str());
  }
}

void
Fib::remove(const string& name, const FaceId& faceId)
{
  Cd cd(name);

  auto it = std::find_if(m_fib.begin(), m_fib.end(), [&](FibEntry& entry) { return entry.m_cd == cd; });
  if (it != m_fib.end()) {
    FibEntry::NextHop nextHop(faceId);
    if (it->m_nextHops.erase(nextHop) > 0) {
      INFO("FIB entry FaceID=%llu CD=%s remoeved", faceId, cd.toUri().c_str());
    }
    if (it->m_nextHops.empty()) {
      m_fib.erase(it);
    }
  }
}

void
Fib::remove(const string& name)
{
  Cd cd(name);

  auto it = std::find_if(m_fib.begin(), m_fib.end(), [&](FibEntry& entry) { return entry.m_cd == cd; });
  if (it != m_fib.end()) {
    INFO("FIB entry CD=%s remoeved", cd.toUri().c_str());
    m_fib.erase(it);
  }
}

void
Fib::remove(const FaceId& faceId)
{
  FibEntry::NextHop nextHop(faceId);

  auto it = m_fib.begin();
  while (it != m_fib.end()) {
    if (it->m_nextHops.erase(nextHop) > 0) {
      INFO("FIB entry FaceID=%llu CD=%s remoeved", faceId, it->m_cd.toUri().c_str());
    }
    if (it->m_nextHops.empty()) {
      it = m_fib.erase(it);
    } else {
      it++;
    }
  }
}

void
Fib::clear()
{
  m_fib.clear();
  INFO("all FIB entry remoeved");
}

set<FaceId>
Fib::match(const Cd& cd) const
{
  set<FaceId> faceIds;

  ssize_t size = -1;
  auto longest = m_fib.cend();

  for (auto it = m_fib.cbegin(); it != m_fib.cend(); it++) {
    if (it->m_cd.isPrefixOf(cd)) { 
      if (size < ssize_t(it->m_cd.size())) {
        size = it->m_cd.size();
        longest = it;
      }
    }
  }

  if (longest != m_fib.cend()) {
    auto nextHop = std::min_element(longest->m_nextHops.begin(), longest->m_nextHops.end(),
                                    [](const FibEntry::NextHop& lhs, const FibEntry::NextHop& rhs) {
                                      return (lhs.m_cost < rhs.m_cost);
                                    });
    faceIds.insert(nextHop->m_faceId);
    INFO("Packet CD=%s : FIB entry match FaceID=%llu CD=%s",
         cd.toUri().c_str(), nextHop->m_faceId, longest->m_cd.toUri().c_str());
  }

  if (faceIds.size() == 0) {
    INFO("Packet CD=%s : FIB entry no match", cd.toUri().c_str());
  }

  return faceIds;
}

void
Fib::dump(vector<string>& lines) const
{
  lines.clear();
  for (auto it = m_fib.cbegin(); it != m_fib.cend(); it++) {
    lines.push_back(it->toString());
  }
}

} // namespace router
} // namespace fcopss

