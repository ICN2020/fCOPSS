/*
  st-impl.hpp

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
#ifndef _FCOPSS_ROUTER_ST_IMPL_HPP_
#define _FCOPSS_ROUTER_ST_IMPL_HPP_

#include <fcopss/common.hpp>

#include "st.hpp"
#include "st-entry.hpp"

namespace fcopss {
namespace router {

class StImpl final : public St
{
public:
  StImpl(io_service& ioSerivce, const time_duration& exprieTime);

  virtual std::tuple<bool, Cd>
  add(const Cd& cd, const FaceId& id) override;

  virtual void
  remove(const Cd& cd, const FaceId& id) override;

  virtual void
  remove(const Cd& cd) override;

  virtual void
  remove(const FaceId& id) override;

  virtual void
  clear() override;
 
  virtual set<FaceId>
  match(const Cd& cd) const override;

  static bool
  matchRegex(const Cd& entryCd, const Cd& inputCd);

  virtual void
  dump(vector<string>& lines) const override;

private:
  void
  onExpired(Cd cd, FaceId id, const boost::system::error_code& error);

  static bool
  isEqualValue(const Block& entry, const Block& input);

  static void
  componentDebugPrint(const Block& entry, const Block& input);

private:
  io_service& m_ioService;
  time_duration m_expireTime;
  list<StEntry> m_st;
};

} // namespace router
} // namespace fcopss

#endif // _FCOPSS_ROUTER_ST_IMPL_HPP_
