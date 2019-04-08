/*
  st-entry.cpp

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
#include "st-entry.hpp"

#include <boost/date_time.hpp>
#include <boost/date_time/c_local_time_adjustor.hpp>

namespace placeholders = boost::asio::placeholders;

using adjustor = boost::date_time::c_local_adjustor<boost::posix_time::ptime>;

namespace fcopss {
namespace router {

StEntry::NextHop::NextHop()
  : m_faceId(0)
{
}

StEntry::NextHop::NextHop(const FaceId& id)
  : m_faceId(id)
{
}

StEntry::NextHop::NextHop(io_service& ioService, const time_duration& expireTime, const StExpiredCallback& onStExipred, const FaceId& id)
  : m_faceId(id)
{
  m_expireTimer = std::make_shared<deadline_timer>(ioService);
  m_expireTimer->expires_from_now(expireTime);
  m_expireTimer->async_wait(boost::bind(onStExipred, placeholders::error));
}

bool
StEntry::NextHop::operator==(const NextHop& nextHop) const
{
  return (m_faceId == nextHop.m_faceId);
}

bool
StEntry::NextHop::operator!=(const NextHop& nextHop) const
{
  return (m_faceId != nextHop.m_faceId);
}

bool
StEntry::NextHop::operator<(const NextHop& nextHop) const
{
  return (m_faceId < nextHop.m_faceId);
}

bool
StEntry::NextHop::operator>(const NextHop& nextHop) const
{
  return (m_faceId > nextHop.m_faceId);
}

string
StEntry::NextHop::toString() const
{
  stringstream ss;

  auto facet = new boost::posix_time::time_facet("%Y-%m-%d %H:%M:%S");
  ss.imbue(std::locale(std::cout.getloc(), facet));
  // stream will delete facet.
  
  ss << "(" << m_faceId << ",";
  if (m_expireTimer->expires_at().is_special()) {
    ss << "INFINITY";
  } else {
    boost::posix_time::ptime expire = adjustor::utc_to_local(m_expireTimer->expires_at());
    ss << expire;
  }
  ss  << ")";

  return ss.str();
}


StEntry::StEntry()
{
}

StEntry::StEntry(const Cd& cd, const NextHop& nextHop)
  : m_cd(cd)
{
  m_nextHops.insert(nextHop);
}


bool
StEntry::operator==(const StEntry& stEntry) const
{
  return ((m_cd == stEntry.m_cd) && (m_nextHops == stEntry.m_nextHops));
}

bool
StEntry::operator!=(const StEntry& stEntry) const
{
  return !operator==(stEntry);
}

string
StEntry::toString() const
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
