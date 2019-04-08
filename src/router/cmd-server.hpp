/*
  cmd-server.hpp

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
#ifndef _FCOPSS_ROUTER_CMD_SERVER_HPP_
#define _FCOPSS_ROUTER_CMD_SERVER_HPP_

#include <fcopss/common.hpp>

#include "face.hpp"

namespace fcopss {
namespace router {

using CmdArg = std::map<string, string>;

class FaceManager;
class Fib;
class St;

class CmdServer final : public noncopyable
{
public:

  class Error : public std::runtime_error
  {
  public:
    explicit
    Error(const string& what)
      : std::runtime_error(what)
    {
    }
  };

  CmdServer(
    boost::asio::io_service& ioService,
    uint16_t ctrlPort,
    uint16_t routerPort,
    const time_duration& receiveTimeout,
    const time_duration& connectTimeout,
    FaceManager& faceManager,
    Fib& fib,
    St& st);

  void
  start();

private:
  enum CmdType
  {
    FibAdd = 1,
    FibDel = 2,
    StDel = 3,
    FaceDel = 4,
    StDump = 5,
    FibDump = 6,
    FaceDump = 7
  };

  void
  asyncAccept();

  void
  handleAccept(const boost::system::error_code& error);

  void
  asyncReceive();

  void
  handleReceiveTimeout(const boost::system::error_code& error);

  void
  handleReceive(const boost::system::error_code& error, size_t bytes_transferred);

  CmdType
  readCmd(size_t length, CmdArg& arg);

  void
  fibAdd(const CmdArg& cmdArg);

  void
  fibAdd(const string& name, const string& type, const string& ip, uint16_t port, uint32_t cost);

  void
  fibAddUdpFace(const string& name, const string& ip, uint16_t port, uint32_t cost);

  void
  fibAddTcpFace(const string& name, const string& ip, uint16_t port, uint32_t cost);

  void
  handleConnectTimeout(const string& name, const string& ip, uint16_t port, uint32_t cost, const boost::system::error_code& error);

  void
  handleConnect(const string& name, const string& ip, uint16_t port, uint32_t cost, const boost::system::error_code& error);

  void
  fibDel(const CmdArg& cmdArg);

  void
  fibDel(const string& name);

  void
  fibDel(const string& name, const string& type, const string& ip, uint16_t port);

  void
  fibDel();

  void
  stDel();

  void
  faceDel(const CmdArg& cmdArg);

  void
  faceDel(const FaceId& faceId);

  void
  stDump();

  void
  fibDump();

  void
  faceDump();

  void
  sendOk();

  void
  sendErr(const string& reason);

  void
  sendReply(const string& message);

  void
  handleSend(shared_ptr<string> reply, const boost::system::error_code& error);

private:
  io_service& m_ioService;
  tcp::endpoint m_endPoint;
  tcp::acceptor m_acceptor;
  tcp::socket m_socket;
  time_duration m_receiveTimeout;
  time_duration m_connectTimeout;
  uint16_t m_port;
  FaceManager& m_faceManager;
  Fib& m_fib;
  St& m_st;
  boost::asio::streambuf m_receiveBuffer;
  boost::asio::deadline_timer m_receiveTimer;
  boost::asio::deadline_timer m_connectTimer;
  unique_ptr<tcp::socket> m_connection;
};

} // namespace router
} // namespace fcopss

#endif // _FCOPSS_ROUTER_CMD_SERVER_HPP_

