/*
  cmd.cpp

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

#include <fcopss/cd.hpp>

#include "cmd.hpp"

namespace fcopss {
namespace rtctrl {

Cmd::Cmd(int argc, char* argv[], uint16_t ctrlPort)
  : m_argc(argc), m_argv(argv), m_ioService(), m_socket(m_ioService), m_ctrlPort(ctrlPort)
{
}

void
Cmd::setParamName(Request& request, const string& name)
{
  try {
    Cd cd(name);
    request.m_param["NAME"] = cd.toUri();
  } catch (...) {
    BOOST_THROW_EXCEPTION(Error("illegal cmd arg: " + name));
  }
}

void
Cmd::setParamType(Request& request, const string& type)
{
  if (!strcasecmp(type.c_str(), "tcp")) {
    request.m_param["TYPE"] = "tcp";
  } else if (!strcasecmp(type.c_str(), "udp")) {
    request.m_param["TYPE"] = "udp";
  } else {
    BOOST_THROW_EXCEPTION(Error("illegal cmd arg: " + type));
  }
}

void
Cmd::setParamRemote(Request& request, const string& remote, uint16_t defaultPort)
{
  string port;
  string ip;
  getIpPort(remote, defaultPort, ip, port);

  boost::system::error_code error;
  boost::asio::ip::address_v4::from_string(ip, error);
  if (error) {
    boost::asio::ip::resolver_query_base::flags flags;
    flags = boost::asio::ip::resolver_query_base::numeric_service;
    tcp::resolver resolver(m_ioService);
    tcp::resolver::query query(tcp::v4(), remote, port, flags);
    boost::system::error_code ec;
    tcp::resolver::iterator it = resolver.resolve(query, ec);
    if (ec) {
      BOOST_THROW_EXCEPTION(Error("cannot resolve host name: " + ec.message()));
    }
    tcp::resolver::iterator end;
    if (it == end) {
      BOOST_THROW_EXCEPTION(Error("cannot resolve host name"));
    }
    ip = it->endpoint().address().to_string();
  }

  request.m_param["IP"] = ip;
  request.m_param["PORT"] = port;
}

void
Cmd::getIpPort(const string& remote, uint16_t defaultPort, string& ip, string& port)
{
  ip = remote;
  std::remove_if(ip.begin(), ip.end(), [](const char& c){return isspace(c);});

  vector<string> peer;
  boost::split(peer, ip, boost::is_any_of(":"));
  if (peer.size() == 1) {
    port = std::to_string(defaultPort);
  } else if (peer.size() == 2) {
    ip = peer[0];
    port = peer[1];
  } else {
    BOOST_THROW_EXCEPTION(Error("illegal cmd arg"));
  }
}

void
Cmd::execute()
{
  parse(m_argc, m_argv);

  connect();

  for (auto request = m_requests.begin(); request != m_requests.end(); request++) {
    send(*request);
    Response response;
    receive(*request, response);
    m_responses.push_back(response);
  }

  disconnect();

  show();
}

void
Cmd::connect()
{
  boost::system::error_code error;
  m_socket.connect(tcp::endpoint(boost::asio::ip::address_v4::from_string("127.0.0.1"), m_ctrlPort), error);

  if (error) {
    BOOST_THROW_EXCEPTION(Error("cannot connect to local router"));
  }
}

void
Cmd::disconnect()
{
  boost::system::error_code error;
  m_socket.close(error);
}

void
Cmd::send(const Request& request)
{
  boost::system::error_code error;
  boost::asio::write(m_socket, boost::asio::buffer(request.format()), error);

  if (error) {
    disconnect();
    BOOST_THROW_EXCEPTION(Error("cannot send to local router"));
  }
}

void
Cmd::receive(const Request& request, Response& response)
{
  boost::system::error_code error;
  size_t length = boost::asio::read_until(m_socket, m_buffer, "\r\n\r\n", error);

  if (error) {
    BOOST_THROW_EXCEPTION(Error("cannot receive from local router"));
  }

  std::istringstream iss(string(boost::asio::buffer_cast<const char*>(m_buffer.data()), length));
  m_buffer.consume(length);

  response.setHeader(iss);
}

void
Cmd::show() const
{
  for (size_t i = 0; i < m_responses.size(); i++) {
    std::cout << "COMMAND: " << m_requests[i].m_cmd << std::endl;
    for (auto it = m_requests[i].m_param.cbegin(); it != m_requests[i].m_param.cend(); it++) {
      std::cout << it->first << ": " << it->second << std::endl;
    }

    std::cout << "RESULT: " << m_responses[i].m_result << std::endl;
    if (m_responses[i].m_result != "OK") {
      std::cout << "REASON: " << m_responses[i].m_reason << std::endl;
    }

    for (auto it = m_responses[i].m_header.cbegin(); it != m_responses[i].m_header.cend(); it++) {
      std::cout << it->first << ": " << it->second << std::endl;
    }

    for (auto it = m_responses[i].m_body.cbegin(); it != m_responses[i].m_body.cend(); it++) {
      std::cout << *it << std::endl;
    }
  }
}


} // namespace rtctrl
} // namespace fcopss

