/*
  fib.hpp

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
#ifndef _FCOPSS_ROUTER_FIB_HPP_
#define _FCOPSS_ROUTER_FIB_HPP_

#include <fcopss/common.hpp>

#include "fib-entry.hpp"

namespace fcopss {
namespace router {

class Fib final : noncopyable
{
public:
  void
  add(const string& name, const FaceId& faceId, uint32_t cost);

  void
  remove(const string& name, const FaceId& faceId);

  void
  remove(const string& name);

  void
  remove(const FaceId& faceId);

  void
  clear();

  set<FaceId>
  match(const Cd& cd) const;

  void
  dump(vector<string>& lines) const;

private:
  list<FibEntry> m_fib;
};

} // namespace router
} // namespace fcopss

#endif // _FCOPSS_ROUTER_FIB_HPP_
