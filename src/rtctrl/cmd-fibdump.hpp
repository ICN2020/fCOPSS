/*
  cmd-fibdump.hpp

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
#ifndef _FCOPSS_RTCTRL_CMD_FIBDUMP_HPP_
#define _FCOPSS_RTCTRL_CMD_FIBDUMP_HPP_

#include <fcopss/common.hpp>

#include "cmd.hpp"

#include <set>

namespace fcopss {
namespace rtctrl {

class CmdFibDump : public Cmd
{
public:
  CmdFibDump(int argc, char* argv[], uint16_t ctrlPort);

  void
  parse(int argc, char* argv[]) override;

  void
  receive(const Request& request, Response& response) override;

  void
  show() const override;

private:
  void
  receiveBody(Response& response, uint32_t count);

  void
  getFaceId(const string& fib, std::set<string>& id) const;

  bool
  matchFaceId(const string& face, const std::set<string>& id) const;

private:
  uint32_t m_fibCount;
  uint32_t m_faceCount;
};

} // namespace rtctrl
} // namespace fcopss

#endif // _FCOPSS_RTCTRL_CMD_FIBDUMP_HPP_


