/*
  rtctrl.hpp

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
#ifndef _FCOPSS_RTCTRL_RTCTRL_HPP_
#define _FCOPSS_RTCTRL_RTCTRL_HPP_

#include <fcopss/common.hpp>
#include <fcopss/config.hpp>

#include "cmd.hpp"
#include "cmd-fibadd.hpp"
#include "cmd-fibdel.hpp"
#include "cmd-fibdump.hpp"
#include "cmd-stdel.hpp"
#include "cmd-stdump.hpp"
#include "cmd-facedel.hpp"
#include "cmd-facedump.hpp"

namespace fcopss {
namespace rtctrl {

class Rtctrl
{
public:
  enum CmdType
  {
    FibAdd = 1,
    FibDel = 2,
    StDel = 3,
    StDump = 4,
    FibDump = 5,
    FaceDel = 6,
    FaceDump = 7,
  };

  Rtctrl(const Config& config);

  void
  run(int argc, char* argv[]);

  Cmd*
  createCmd(int argc, char* argv[]);

  CmdType
  parseCmdType(int argc, char* argv[]);

  void
  usage() const;

private:
  uint16_t m_routerPort;
  uint16_t m_ctrlPort;
};

} // namespace rtctrl
} // namespace fcopss

#endif // _FCOPSS_RTCTRL_RTCTRL_HPP_

