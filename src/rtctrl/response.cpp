/*
  response.cpp

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

#include "response.hpp"

#include <boost/algorithm/string.hpp>

namespace fcopss {
namespace rtctrl {

void
Response::setHeader(std::istringstream& iss)
{
  string line;
  if (std::getline(iss, line)) {
    boost::trim_right_if(line, boost::is_any_of("\r"));
    if (line == "OK") {
      m_result = "OK";
    } else {
      m_result = "ERR";
      if (line.size() > (m_result.size() + 2)) {
        m_reason = line.substr(m_result.size() + 1);
      }
    }
  }
  while (std::getline(iss, line)) {
    boost::trim_right_if(line, boost::is_any_of("\r"));
    if (!line.empty()) {
      vector<string> kv;
      boost::split(kv, line, boost::is_any_of(":"));
      if (kv.size() == 2) {
        boost::trim(kv[0]);
        boost::trim(kv[1]);
        m_header[kv[0]] = kv[1];
      }
    }
  }
}

} // namespace rtctrl
} // namespace fcopss

