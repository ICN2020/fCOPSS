/*
  fib-entry.cpp

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
#include "fib-entry.hpp"

namespace fcopss {
namespace router {

FibEntry::NextHop::NextHop()
  : m_faceId(0), m_cost(0)
{
}

FibEntry::NextHop::NextHop(const FaceId& faceId)
  : m_faceId(faceId), m_cost(0)
{
}

FibEntry::NextHop::NextHop(const FaceId& faceId, uint32_t cost)
  : m_faceId(faceId), m_cost(cost)
{
}

bool
FibEntry::NextHop::operator==(const NextHop& nextHop) const
{
  return (m_faceId == nextHop.m_faceId);
}
bool
FibEntry::NextHop::operator!=(const NextHop& nextHop) const
{
  return (m_faceId != nextHop.m_faceId);
}

bool
FibEntry::NextHop::operator<(const NextHop& nextHop) const
{
  return (m_faceId < nextHop.m_faceId);
}

bool
FibEntry::NextHop::operator>(const NextHop& nextHop) const
{
  return (m_faceId > nextHop.m_faceId);
}

string
FibEntry::NextHop::toString() const
{
  stringstream ss;
  
  ss << "(" << m_faceId << "," << m_cost << ")";

  return ss.str();
}


FibEntry::FibEntry()
{
}

FibEntry::FibEntry(const Cd& cd)
  : m_cd(cd)
{
}

FibEntry::FibEntry(const Cd& cd, const NextHop& nextHop)
   : m_cd(cd)
{
  m_nextHops.insert(nextHop);
}

bool
FibEntry::operator==(const FibEntry& fibEntry) const
{
  return ((m_cd == fibEntry.m_cd) && (m_nextHops == fibEntry.m_nextHops));
}

bool
FibEntry::operator!=(const FibEntry& fibEntry) const
{
  return !operator==(fibEntry);
}

string
FibEntry::toString() const
{
  stringstream ss;

  ss << m_cd << "=";
  for (auto it = m_nextHops.cbegin(); it != m_nextHops.cend(); it++) {
    ss << it->toString();
  }

  return ss.str();
}

} // namespace router
} // namespace fcopss
