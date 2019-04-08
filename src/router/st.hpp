/*
  st.hpp

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
#ifndef _FCOPSS_ROUTER_ST_HPP_
#define _FCOPSS_ROUTER_ST_HPP_

#include <fcopss/common.hpp>

#include "face.hpp"

namespace fcopss {
namespace router {

class St
{
public:
  virtual std::tuple<bool, Cd>
  add(const Cd& cd, const FaceId& id) = 0;

  virtual void
  remove(const Cd& cd, const FaceId& id) = 0;

  virtual void
  remove(const Cd& cd) = 0;

  virtual void
  remove(const FaceId& id) = 0;

  virtual void
  clear() = 0;
 
  virtual set<FaceId>
  match(const Cd& cd) const = 0;

  virtual void
  dump(vector<string>& lines) const = 0;
};

} // namespace router
} // namespace fcopss

#endif // _FCOPSS_ROUTER_ST_HPP_
