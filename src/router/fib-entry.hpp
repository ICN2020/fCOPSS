/*
  fib-entry.hpp

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
#ifndef _FCOPSS_ROUTER_FIB_ENTRY_HPP_
#define _FCOPSS_ROUTER_FIB_ENTRY_HPP_

#include <fcopss/common.hpp>

#include "face.hpp"

#include <set>

namespace fcopss {
namespace router {

class FibEntry
{
public:
  class NextHop
  {
  public:
    FaceId m_faceId;
    uint32_t m_cost;

    NextHop();
    NextHop(const FaceId& faceId);
    NextHop(const FaceId& faceId, uint32_t cost);
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
  };

public:
  FibEntry();
  FibEntry(const Cd& cd);
  FibEntry(const Cd& cd, const NextHop& nextHop);
  FibEntry(const FibEntry& fibEntry) = default;
  FibEntry(FibEntry&& fibEntry) = default;
  FibEntry& operator=(const FibEntry& fibEntry) = default;
  FibEntry& operator=(FibEntry&& fibEntry) = default;
  bool operator==(const FibEntry& fibEntry) const;
  bool operator!=(const FibEntry& fibEntry) const;

public:
  string
  toString() const;
 
public:
  Cd m_cd;
  std::set<NextHop> m_nextHops;
};

} // namespace router
} // namespace fcopss

#endif // _FCOPSS_ROUTER_FIB_ENTRY_HPP_
