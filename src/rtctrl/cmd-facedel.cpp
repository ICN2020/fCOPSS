/*
  cmd-facedel.cpp

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
#include "cmd-facedel.hpp"

namespace fcopss {
namespace rtctrl {

CmdFaceDel::CmdFaceDel(int argc, char* argv[], uint16_t ctrlPort)
  : Cmd(argc, argv, ctrlPort)
{
}

void
CmdFaceDel::parse(int argc, char* argv[])
{
  Request request;
  request.m_cmd = "FACE-DEL";

  if (argc == 4) {
    for (int i = 2; i < argc; i++) {
      if (!strcmp(argv[i], "-i")) {
        if (++i >= argc) {
          BOOST_THROW_EXCEPTION(Error("illegal cmd arg: name missing ?"));
        }
        setParamFaceID(request, argv[i]);
        continue;
      }
      BOOST_THROW_EXCEPTION(Error("illegal cmd arg: unknown option"));
    }
    if (request.m_param.size() != 1) {
      BOOST_THROW_EXCEPTION(Error("illegal cmd arg: missing/duplicating option"));
    }
    m_requests.push_back(request);
  } else {
    BOOST_THROW_EXCEPTION(Error("illegal cmd arg"));
  }
}

void
CmdFaceDel::setParamFaceID(Request& request, const string& id)
{
  try {
    boost::lexical_cast<uint64_t>(id);
  } catch (...) {
    BOOST_THROW_EXCEPTION(Error("illegal Face ID"));
  }

  request.m_param["FACE"] = id;
}

} // namespace rtctrl
} // namespace fcopss
