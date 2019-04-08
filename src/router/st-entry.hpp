/*
  st-entry.hpp

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
#ifndef _FCOPSS_ROUTER_ST_ENTRY_HPP_
#define _FCOPSS_ROUTER_ST_ENTRY_HPP_

#include <fcopss/common.hpp>

#include <boost/function.hpp>
#include <boost/bind.hpp>

#include "face.hpp"

#include <set>

namespace fcopss {
namespace router {

using StExpiredCallback = boost::function<void(const boost::system::error_code&)>;

class StEntry
{
public:
  class NextHop
  {
  public:
    shared_ptr<deadline_timer> m_expireTimer;
    FaceId m_faceId;

    NextHop();
    NextHop(const FaceId& id);
    NextHop(io_service& ioService, const time_duration& expireTime, const StExpiredCallback& onStExipred, const FaceId& id);
    NextHop(const NextHop& nextHop) = default;
    NextHop(NextHop&& nextHop) = default;
    NextHop& operator=(const NextHop& nextHop) = default;
    NextHop& operator=(NextHop&& nextHop) = default;
    bool operator==(const NextHop& nextHop) const;
    bool operator!=(const NextHop& nextHop) const;
    bool operator<(const NextHop& nextHop) const;
    bool operator>(const NextHop& nextHop) const;

    string
    toString() const;

  private:
    StExpiredCallback m_onStExpired;

  };

public:
  StEntry();
  StEntry(const Cd& cd, const NextHop& nextHop);
  StEntry(const StEntry& stEntry) = default;
  StEntry(StEntry&& stEntry) = default;
  StEntry& operator=(const StEntry& stEntry) = default;
  StEntry& operator=(StEntry&& stEntry) = default;
  bool operator==(const StEntry& stEntry) const;
  bool operator!=(const StEntry& stEntry) const;

public:
  string
  toString() const;
 
public:
  Cd m_cd;
  std::set<NextHop> m_nextHops;
};

} // namespace router
} // namespace fcopss

#endif // _FCOPSS_ROUTER_ST_ENTRY_HPP_
