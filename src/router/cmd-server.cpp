/*
  cmd-server.cpp

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
#include "cmd-server.hpp"
#include "tcp-transport.hpp"
#include "udp-transport.hpp"
#include "face-manager.hpp"
#include "fib.hpp"
#include "st.hpp"

#include <boost/bind.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/algorithm/string.hpp>

#include <fcopss/log.hpp>

namespace asio = boost::asio;
namespace placeholders = boost::asio::placeholders;

using reuse_address = boost::asio::socket_base::reuse_address;
using reuse_port = boost::asio::detail::socket_option::boolean<SOL_SOCKET, SO_REUSEPORT>;

namespace fcopss {
namespace router {

static const string TypeTcp = "tcp";
static const string TypeUdp = "udp";

CmdServer::CmdServer(
  asio::io_service& ioService,
  uint16_t ctrlPort,
  uint16_t routerPort,
  const time_duration& receiveTimeout,
  const time_duration& connectTimeout,
  FaceManager& faceManager,
  Fib& fib,
  St& st)
    : m_ioService(ioService),
      m_endPoint(tcp::v4(), ctrlPort),
      m_acceptor(m_ioService, m_endPoint, true),
      m_socket(m_ioService),
      m_receiveTimeout(receiveTimeout),
      m_connectTimeout(connectTimeout),
      m_port(routerPort),
      m_faceManager(faceManager),
      m_fib(fib),
      m_st(st),
      m_receiveTimer(ioService),
      m_connectTimer(ioService)
{
}

void
CmdServer::start()
{
  asyncAccept();
}

void
CmdServer::asyncAccept()
{
  m_acceptor.async_accept(m_socket, boost::bind(&CmdServer::handleAccept, this, placeholders::error));
}

void
CmdServer::handleAccept(const boost::system::error_code& error)
{
  if (!error) {
    tcp::endpoint remote = m_socket.remote_endpoint();
    DEBUG("connection from %s:%hu", remote.address().to_string().c_str(), remote.port());
    asyncReceive();
  } else {
    ERROR("asio accept error: %s", error.message().c_str());
  }
}

void
CmdServer::asyncReceive()
{
  asio::async_read_until(
    m_socket,
    m_receiveBuffer,
    "\r\n\r\n",
    boost::bind(&CmdServer::handleReceive, this, placeholders::error, placeholders::bytes_transferred)
  );

  m_receiveTimer.expires_from_now(m_receiveTimeout);
  m_receiveTimer.async_wait(bind(&CmdServer::handleReceiveTimeout, this, placeholders::error));
}

void
CmdServer::handleReceiveTimeout(const boost::system::error_code& error)
{
  if (!error) {
    WARN("control command read timeout");
    boost::system::error_code ec;
    m_socket.close(ec);
    asyncAccept();
  }
}

void
CmdServer::handleReceive(const boost::system::error_code& error, size_t bytes_transferred)
{
  m_receiveTimer.cancel();

  try {
    if (!error) {
      CmdArg arg;
      CmdType cmdType = readCmd(bytes_transferred, arg);
      switch (cmdType) {
      case FibAdd:
        fibAdd(arg);
        break;
      case FibDel:
        if (!arg.empty()) {
          fibDel(arg);
        } else {
          fibDel();
        }
        break;
      case StDel:
        stDel();
        break;
      case FaceDel:
        faceDel(arg);
        break;
      case StDump:
        stDump();
        break;
      case FibDump:
        fibDump();
        break;
      case FaceDump:
        faceDump();
        break;
      }
    } else if ((error == asio::error::eof) || (error == asio::error::connection_reset)) {
      DEBUG("%s", error.message().c_str());
      boost::system::error_code ec;
      m_socket.close(ec);
      asyncAccept();
    } else if (error == asio::error::operation_aborted) {
      // case of read timeout
      DEBUG("%s", error.message().c_str());
    } else {
      ERROR("asio read error: %s", error.message().c_str());
      boost::system::error_code ec;
      m_socket.close(ec);
      asyncAccept();
    }
  } catch (const std::runtime_error& e) {
    sendErr(e.what());
    asyncReceive();
  }
}

CmdServer::CmdType
CmdServer::readCmd(size_t length, CmdArg& arg)
{
  std::istringstream iss(string(asio::buffer_cast<const char*>(m_receiveBuffer.data()), length));
  m_receiveBuffer.consume(length);

  CmdType cmdType;

  string line;
  if (std::getline(iss, line)) {
    boost::trim_right_if(line, boost::is_any_of("\r"));
    if (line == "FIB-ADD") {
      cmdType = FibAdd;
    } else if (line == "FIB-DEL") {
      cmdType = FibDel;
    } else if (line == "ST-DEL") {
      cmdType = StDel;
    } else if (line == "FACE-DEL") {
      cmdType = FaceDel;
    } else if (line == "ST-DUMP") {
      cmdType = StDump;
    } else if (line == "FIB-DUMP") {
      cmdType = FibDump;
    } else if (line == "FACE-DUMP") {
      cmdType = FaceDump;
    } else {
      string what = string("unknown control command: ") + line;
      BOOST_THROW_EXCEPTION(Error(what));
    }
  } else {
    BOOST_THROW_EXCEPTION(Error("illegal control command"));
  }

  arg.clear();
  while (std::getline(iss, line)) {
    boost::trim_right_if(line, boost::is_any_of("\r"));
    if (!line.empty()) {
      vector<string> kv;
      boost::split(kv, line, boost::is_any_of(":"));
      if (kv.size() != 2) {
        string what = string("illegal control command arg: ") + line;
        BOOST_THROW_EXCEPTION(Error(what));
      }
      boost::trim(kv[0]);
      boost::trim(kv[1]);
      arg[kv[0]] = kv[1];
    }
  }

  return cmdType;
}

void
CmdServer::fibAdd(const CmdArg& cmdArg)
{
  string name;
  string type;
  string ip;
  string portStr;
  string costStr;

  try {
    name = cmdArg.at("NAME");
    type = cmdArg.at("TYPE");
    ip = cmdArg.at("IP");
    portStr = cmdArg.at("PORT");
    costStr = cmdArg.at("COST");
  } catch (...) {
    string what = string("illegal control command arg");
    BOOST_THROW_EXCEPTION(Error(what));
  }

  if ((type != TypeTcp) && (type != TypeUdp)) {
    string what = string("illegal control command arg");
    BOOST_THROW_EXCEPTION(Error(what));
  }

  uint16_t port = boost::lexical_cast<uint16_t>(portStr);
  uint32_t cost = boost::lexical_cast<uint32_t>(costStr);

  fibAdd(name, type, ip, port, cost);
}

void
CmdServer::fibAdd(const string& name, const string& type, const string& ip, uint16_t port, uint32_t cost)
{
  shared_ptr<Face> face = m_faceManager.find(type, ip, port);
  if (face) {
    m_fib.add(name, face->getId(), cost);
    sendOk();
    asyncReceive();
  } else {
    if (type == TypeTcp) {
      fibAddTcpFace(name, ip, port, cost);
    } else {
      fibAddUdpFace(name, ip, port, cost);
    }
  }
}

void CmdServer::fibAddUdpFace(const string& name, const string& ip, uint16_t port, uint32_t cost)
{
  udp::socket s(m_ioService);
  s.open(udp::v4());
  s.set_option(reuse_address(true));
  s.set_option(reuse_port(true));
  s.bind(udp::endpoint(udp::v4(), m_port));
  s.connect(udp::endpoint(asio::ip::address::from_string(ip), port));

  udp::endpoint local = s.local_endpoint();
  udp::endpoint remote = s.remote_endpoint();
  if (local == remote) {
    boost::system::error_code ec;
    s.close(ec);
    BOOST_THROW_EXCEPTION(Error("local endpoint and remote endpoint are the same"));
  }

  shared_ptr<Transport> transport = UdpTransport::create(std::move(s));
  m_faceManager.createFace(transport);

  fibAdd(name, TypeUdp, ip, port, cost);
}

void
CmdServer::fibAddTcpFace(const string& name, const string& ip, uint16_t port, uint32_t cost)
{
  m_connection.reset(new tcp::socket(m_ioService));
  m_connection->open(tcp::v4());
  m_connection->set_option(reuse_address(true));
  m_connection->set_option(reuse_port(true));
  m_connection->bind(tcp::endpoint(tcp::v4(), m_port));
  m_connection->async_connect(
    tcp::endpoint(asio::ip::address::from_string(ip), port),
    boost::bind(&CmdServer::handleConnect, this, name, ip, port, cost, placeholders::error));
  
  m_connectTimer.expires_from_now(m_connectTimeout);
  m_connectTimer.async_wait(
    bind(&CmdServer::handleConnectTimeout, this, name, ip, port, cost, placeholders::error));
}

void
CmdServer::handleConnectTimeout(const string& name, const string& ip, uint16_t port, uint32_t cost, const boost::system::error_code& error)
{
  if (!error) {
    boost::system::error_code ec;
    m_connection->close(ec);
    m_connection.reset();
  }
}

void
CmdServer::handleConnect(const string& name, const string& ip, uint16_t port, uint32_t cost, const boost::system::error_code& error)
{
  m_connectTimer.cancel();

  string msg = error.message();

  if (!error) {
    tcp::endpoint local = m_connection->local_endpoint();
    tcp::endpoint remote = m_connection->remote_endpoint();
    if (local != remote) {
      shared_ptr<Transport> transport = TcpTransport::create(std::move(*m_connection), m_receiveTimeout);
      m_faceManager.createFace(transport);
      m_connection.reset();
      fibAdd(name, TypeTcp, ip, port, cost);
      return;
    } else {
      msg = "local endpoint and remote endpoint are the same";
    }
  }

  DEBUG("Remote=%s:%hu MSG=%s", ip.c_str(), port, msg.c_str());

  if (error == asio::error::operation_aborted) {
    msg += " (connect timeout)";
  } else {
    boost::system::error_code ec;
    m_connection->close(ec);
    m_connection.reset();
  }

  sendErr(msg);
  asyncReceive();
}

void
CmdServer::fibDel(const CmdArg& cmdArg)
{
  string name;
  string type;
  string ip;
  string portStr;

  try { name = cmdArg.at("NAME"); } catch (...) {}
  try { type = cmdArg.at("TYPE"); } catch (...) {}
  try { ip = cmdArg.at("IP"); } catch (...) {}
  try { portStr = cmdArg.at("PORT"); } catch (...) {}

  if (name.empty()) {
    string what = string("illegal control command arg");
    BOOST_THROW_EXCEPTION(Error(what));
  }

  if (type.empty() && ip.empty() && portStr.empty()) {
    fibDel(name);
  } else if (!type.empty() && !ip.empty() && !portStr.empty()) {
    if ((type != TypeTcp) && (type != TypeUdp)) {
      string what = string("illegal control command arg");
      BOOST_THROW_EXCEPTION(Error(what));
    }
    uint16_t port = boost::lexical_cast<uint16_t>(portStr);
    fibDel(name, type, ip, port);
  } else {
    string what = string("illegal control command arg");
    BOOST_THROW_EXCEPTION(Error(what));
  }
}

void
CmdServer::fibDel(const string& name)
{
  m_fib.remove(name);
  sendOk();
  asyncReceive();
}

void
CmdServer::fibDel(const string& name, const string& type, const string& ip, uint16_t port)
{
  
  shared_ptr<Face> face = m_faceManager.find(type, ip, port);
  if (face) {
    m_fib.remove(name, face->getId());
    sendOk();
  } else {
    sendErr("face not found");
  }
  asyncReceive();
}

void
CmdServer::fibDel()
{
  m_fib.clear();
  sendOk();
  asyncReceive();
}

void
CmdServer::stDel()
{
  m_st.clear();
  sendOk();
  asyncReceive();
}

void
CmdServer::faceDel(const CmdArg& cmdArg)
{
  string face;

  try {
    face = cmdArg.at("FACE");
  } catch (...) {
    string what = string("illegal control command arg");
    BOOST_THROW_EXCEPTION(Error(what));
  }

  FaceId faceId = boost::lexical_cast<FaceId>(face);

  faceDel(faceId);
}

void
CmdServer::faceDel(const FaceId& faceId)
{
  shared_ptr<Face> face = m_faceManager.find(faceId);
  if (face) {
    face->shutdown();
  }
  sendOk();
  asyncReceive();
}

void
CmdServer::stDump()
{
  vector<string> sts;
  m_st.dump(sts);

  stringstream ss;
  ss << "OK" << "\r\n";

  ss << "ST-COUNT: " << sts.size() << "\r\n";
  ss << "\r\n";

  for (auto st = sts.cbegin(); st != sts.cend(); st++) {
    ss << *st << "\r\n";
  }

  sendReply(ss.str());  

  asyncReceive();
}

void
CmdServer::fibDump()
{
  vector<string> fibs;
  m_fib.dump(fibs);

  stringstream ss;
  ss << "OK" << "\r\n";

  ss << "FIB-COUNT: " << fibs.size() << "\r\n";
  ss << "\r\n";

  for (auto fib = fibs.cbegin(); fib != fibs.cend(); fib++) {
    ss << *fib << "\r\n";
  }

  sendReply(ss.str());  

  asyncReceive();
}

void
CmdServer::faceDump()
{
  vector<string> faces;
  m_faceManager.dump(faces);

  stringstream ss;
  ss << "OK" << "\r\n";

  ss << "FACE-COUNT: " << faces.size() << "\r\n";
  ss << "\r\n";

  for (auto face = faces.cbegin(); face != faces.cend(); face++) {
    ss << *face << "\r\n";
  }

  sendReply(ss.str());

  asyncReceive();
}

void
CmdServer::sendOk()
{
  sendReply("OK\r\n\r\n");
}

void
CmdServer::sendErr(const string& reason)
{
  sendReply(string("ERR ") + reason + "\r\n\r\n");
}

void
CmdServer::sendReply(const string& message)
{
  shared_ptr<std::string> reply(new std::string(message));
  
  asio::async_write(
    m_socket,
    asio::buffer(*reply),
    boost::bind(&CmdServer::handleSend, this, reply, placeholders::error));
}

void
CmdServer::handleSend(shared_ptr<string> reply, const boost::system::error_code& error)
{
  if (error) {
    ERROR("asio write error: %s", error.message().c_str());
  }
}

} // namespace router
} // namespace fcopss
