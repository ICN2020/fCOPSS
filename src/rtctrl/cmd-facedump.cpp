/*
  cmd-facedump.cpp

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
#include "cmd-facedump.hpp"

namespace fcopss {
namespace rtctrl {

CmdFaceDump::CmdFaceDump(int argc, char* argv[], uint16_t ctrlPort)
  : Cmd(argc, argv, ctrlPort)
{
}

void
CmdFaceDump::parse(int argc, char* argv[])
{
  if (argc == 2) {
    Request request;
    request.m_cmd = "FACE-DUMP";
    m_requests.push_back(request);
  } else {
    BOOST_THROW_EXCEPTION(Error("illegal cmd arg"));
  }
}

void
CmdFaceDump::receive(const Request& request, Response& response)
{
  Cmd::receive(request, response);

  uint32_t faceCount = boost::lexical_cast<uint32_t>(response.m_header["FACE-COUNT"]);
  receiveBody(response, faceCount);
}

void
CmdFaceDump::receiveBody(Response& response, uint32_t count)
{
  for (uint32_t i = 0; i < count; i++) {
    boost::system::error_code error;
    size_t length = boost::asio::read_until(m_socket, m_buffer, "\r\n", error);
    if (error) {
      BOOST_THROW_EXCEPTION(Error("cannot receive from local router"));
    }
    string line = string(boost::asio::buffer_cast<const char*>(m_buffer.data()), length);
    m_buffer.consume(length);
    boost::trim_right_if(line, boost::is_any_of("\n"));
    boost::trim_right_if(line, boost::is_any_of("\r"));
    response.m_body.push_back(line);
  }
}


} // namespace rtctrl
} // namespace fcopss
