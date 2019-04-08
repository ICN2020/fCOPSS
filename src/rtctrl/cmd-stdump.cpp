/*
  cmd-stdump.cpp

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
#include "cmd-stdump.hpp"

namespace fcopss {
namespace rtctrl {

CmdStDump::CmdStDump(int argc, char* argv[], uint16_t ctrlPort)
  : Cmd(argc, argv, ctrlPort), m_stCount(0), m_faceCount(0)
{
}

void
CmdStDump::parse(int argc, char* argv[])
{
  if (argc == 2) {
    Request request;
    request.m_cmd = "ST-DUMP";
    m_requests.push_back(request);
    request.m_cmd = "FACE-DUMP";
    m_requests.push_back(request);
  } else {
    BOOST_THROW_EXCEPTION(Error("illegal cmd arg"));
  }
}

void
CmdStDump::receive(const Request& request, Response& response)
{
  Cmd::receive(request, response);

  if (request.m_cmd == "ST-DUMP") {
    m_stCount = boost::lexical_cast<uint32_t>(response.m_header["ST-COUNT"]);
    receiveBody(response, m_stCount);
  } else {
    m_faceCount = boost::lexical_cast<uint32_t>(response.m_header["FACE-COUNT"]);
    receiveBody(response, m_faceCount);
  }
}

void
CmdStDump::receiveBody(Response& response, uint32_t count)
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

void
CmdStDump::show() const
{
  std::set<string> id;

  if ((m_responses[0].m_result == "OK") && (m_responses[1].m_result == "OK")) {
    std::cout << "ST-COUNT: " << m_stCount << std::endl;
    for (uint32_t i = 0; i < m_stCount; i++) {
      std::cout << m_responses[0].m_body[i] << std::endl;
      getFaceId(m_responses[0].m_body[i], id);
    }
    std::cout << "FACE-COUNT: " << id.size() << std::endl;
    for (uint32_t i = 0; i < m_faceCount; i++) {
      if (matchFaceId(m_responses[1].m_body[i], id)) {
        std::cout << m_responses[1].m_body[i] << std::endl;
      }
    }
  } else {
    std::cout << "COMMAND: " << m_requests[0].m_cmd << std::endl;
    std::cout << "RESULT: " << m_responses[0].m_result << std::endl;
    std::cout << "REASON: " << m_responses[0].m_reason << std::endl;
    std::cout << "COMMAND: " << m_requests[1].m_cmd << std::endl;
    std::cout << "RESULT: " << m_responses[1].m_result << std::endl;
    std::cout << "REASON: " << m_responses[2].m_reason << std::endl;
  }
}

void
CmdStDump::getFaceId(const string& st, std::set<string>& id) const
{
  string rhs = st.substr(st.find("=(") + 1);
  vector<string> v;
  boost::split(v, rhs, boost::is_any_of("(,"));
  for (auto i = v.begin(); i != v.end(); i++) {
    try {
      boost::lexical_cast<uint64_t>(*i);
      id.insert(*i);
    } catch (...) {}
  }
}

bool
CmdStDump::matchFaceId(const string& face, const std::set<string>& id) const
{
  string i = face.substr(0, face.find("=("));
  std::set<string>::const_iterator it = id.find(i);
  return (it != id.cend());
}


} // namespace rtctrl
} // namespace fcopss
