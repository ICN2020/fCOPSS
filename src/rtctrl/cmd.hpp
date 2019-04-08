/*
  cmd.hpp

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
#ifndef _FCOPSS_RTCTRL_CMD_HPP_
#define _FCOPSS_RTCTRL_CMD_HPP_

#include <fcopss/common.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/lexical_cast.hpp>

#include "error.hpp"
#include "request.hpp"
#include "response.hpp"

namespace fcopss {
namespace rtctrl {

class Reply;

class Cmd
{
public:
  Cmd(int argc, char* argv[], uint16_t ctrlPort);

  void
  execute();

protected:
  virtual void
  parse(int argc, char* argv[]) = 0;

  void
  setParamName(Request& request, const string& name);

  void
  setParamType(Request& request, const string& type);

  void
  setParamRemote(Request& request, const string& remote, uint16_t defaultPort);

  void
  connect();

  void
  disconnect();

  virtual void
  send(const Request& request);

  virtual void
  receive(const Request& request, Response& response);

  virtual void
  show() const;

private:
  void
  getIpPort(const string& remote, uint16_t defaultPort, string& ip, string& port);

protected:
  int m_argc;
  char** m_argv;
  io_service m_ioService;
  tcp::socket m_socket;
  uint16_t m_ctrlPort;
  vector<Request> m_requests;
  vector<Response> m_responses;
  boost::asio::streambuf m_buffer;
};

} // namespace rtctrl
} // namespace fcopss

#endif // _FCOPSS_RTCTRL_CMD_HPP_

