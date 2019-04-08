/*
  cmd-fibdel.cpp

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
#include "cmd-fibdel.hpp"

namespace fcopss {
namespace rtctrl {

CmdFibDel::CmdFibDel(int argc, char* argv[], uint16_t ctrlPort, uint16_t routerPort)
  : Cmd(argc, argv, ctrlPort), m_routerPort(routerPort)
{
}

void
CmdFibDel::parse(int argc, char* argv[])
{
  Request request;
  request.m_cmd = "FIB-DEL";

  if (argc == 2) {
    m_requests.push_back(request);
  } else if (argc == 4) {
    parseNameOnly(request, argc, argv);
  } else if (argc == 8) {
    parseFull(request, argc, argv);
  } else {
    BOOST_THROW_EXCEPTION(Error("illegal cmd arg"));
  }
}

void
CmdFibDel::parseNameOnly(Request& request, int argc, char* argv[])
{
  for (int i = 2; i < argc; i++) {
    if (!strcmp(argv[i], "-n")) {
      if (++i >= argc) {
        BOOST_THROW_EXCEPTION(Error("illegal cmd arg: name missing ?"));
      }
      setParamName(request, argv[i]);
      continue;
    }
    BOOST_THROW_EXCEPTION(Error("illegal cmd arg: unknown option"));
  }

  if (request.m_param.size() != 1) {
    BOOST_THROW_EXCEPTION(Error("illegal cmd arg: missing/duplicating option"));
  }

  m_requests.push_back(request);
}

void
CmdFibDel::parseFull(Request& request, int argc, char* argv[])
{
  for (int i = 2; i < argc; i++) {
    if (!strcmp(argv[i], "-n")) {
      if (++i >= argc) {
        BOOST_THROW_EXCEPTION(Error("illegal cmd arg: name missing ?"));
      }
      setParamName(request, argv[i]);
      continue;
    }
    if (!strcmp(argv[i], "-t")) {
      if (++i >= argc) {
        BOOST_THROW_EXCEPTION(Error("illegal cmd arg: type missing ?"));
      }
      setParamType(request, argv[i]);
      continue;
    }
    if (!strcmp(argv[i], "-r")) {
      if (++i >= argc) {
        BOOST_THROW_EXCEPTION(Error("illegal cmd arg: remote missing ?"));
      }
      setParamRemote(request, argv[i], m_routerPort);
      continue;
    }
    BOOST_THROW_EXCEPTION(Error("illegal cmd arg: unknown option"));
  }

  if (request.m_param.size() != 4) {
    BOOST_THROW_EXCEPTION(Error("illegal cmd arg: missing/duplicating option"));
  }

  m_requests.push_back(request);
}

} // namespace rtctrl
} // namespace fcopss
