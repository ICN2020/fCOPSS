/*
  udp-transport.cpp

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
#include <fcopss/udp-transport.hpp>
#include <fcopss/config.hpp>
#include <fcopss/tlv.hpp>
#include <fcopss/log.hpp>

namespace fcopss {
namespace client {

UdpTransport::UdpTransport(io_service& ioService)
  : Transport(ioService)
  , m_socket(ioService)
{
  Config config;
  stringstream portNumber;
  portNumber << config.routerPort();
  m_routerPort = portNumber.str();
  
  getEndPoints(); 
  INFO("Opening socket...");
  m_socket.open(udp::v4());
}

UdpTransport::~UdpTransport()
{
  close();
}

void
UdpTransport::getEndPoints()
{
  INFO("Resolving Endpoints...");
  udp::resolver r(m_ioService);
  udp::resolver::query q("localhost", m_routerPort);  // connect to localhost at the specified port
  udp::resolver::iterator i = r.resolve(q);
  udp::resolver::iterator end;

  if (i != end)
  {
    m_endpoint = *i;
  }
  else
  {
    ERROR("Error while resolving localhost");
    BOOST_THROW_EXCEPTION(Error("Error while resolving localhost"));
  }

  DEBUG("Remote endpoint is at %s at port %hu", 
        m_endpoint.address().to_string().c_str(), m_endpoint.port());
}

void
UdpTransport::close()
{
  if (m_socket.is_open())
  {
    INFO("Closing socket...");
    m_socket.close();
  }
}

void 
UdpTransport::asyncWrite(const PubToRp& pub)
{
  const Block& wire = pub.wireEncode();
  shared_ptr<UdpTransport> self(shared_from_this());
  
  INFO("Sending Pub packet...");
  DEBUG("Wire Type: %d\nWire Length: %lu", wire.type(), wire.value_size());
  m_socket.async_send_to(
    boost::asio::buffer(wire.wire(), wire.size()),
    m_endpoint,
    [this, self] (const boost::system::error_code& error, size_t bytesTransferred) {
      if (error)  
      {
        if (error == boost::system::errc::operation_canceled) // operation explicitly cancelled
        {
          WARN("async_write (Pub) operation explicitly canceled");
          return;
        }

        close();
        ERROR("Error while sending to router (forwarder)");
        BOOST_THROW_EXCEPTION(Error(error, "Error while sending to router (forwarder)"));
      }

      INFO("Pub Packet (%ld bytes) Sent!", bytesTransferred);
    }
  );
}

void 
UdpTransport::asyncWrite(const Sub& sub)
{
  const Block& wire = sub.wireEncode();
  shared_ptr<UdpTransport> self(shared_from_this());
  
  INFO("Sending Sub packet...");
  DEBUG("Wire Type: %d\nWire Length: %lu", wire.type(), wire.value_size());
  m_socket.async_send_to(
    boost::asio::buffer(wire.wire(), wire.size()),
    m_endpoint,
    [this, self] (const boost::system::error_code& error, size_t bytesTransferred) {
      if (error)  
      {
        if (error == boost::system::errc::operation_canceled) // operation explicitly cancelled
        {
          WARN("async_write (Sub) operation explicitly canceled");
          return;
        }

        close();
        ERROR("Error while sending to router (forwarder)");
        BOOST_THROW_EXCEPTION(Error(error, "Error while sending to router (forwarder)"));
      }

      INFO("Sub Packet (%ld bytes) Sent!", bytesTransferred);
    }
  );
}

void 
UdpTransport::asyncRead()
{
  shared_ptr<UdpTransport> self(shared_from_this());
  
  INFO("Listening for PubFromRp messages...");
  m_socket.async_receive_from(
    boost::asio::buffer(m_inputBuffer, MAX_PACKET_SIZE), 
    m_endpoint,
    [this, self] (const boost::system::error_code& error, size_t bytesReceived) {
      PubFromRp emptyPub;
      if (error)
      {
        if (error == boost::system::errc::operation_canceled) // operation explicitly cancelled
        {
          WARN("async_receive operation explicitly canceled");
          recvCallback(emptyPub, error);
          return;
        }
        
        close();
        ERROR("Error while receiving from router (forwarder)");
        recvCallback(emptyPub, error);
        return;
      }
      
      INFO("Message Received (%ld bytes)!", bytesReceived);

      bool isSuccess = false;
      Block wire;
      tie(isSuccess, wire) = Block::fromBuffer(m_inputBuffer, bytesReceived);

      if (!isSuccess)
      {
        close();
        ERROR("Failed to decode received message");
        recvCallback(emptyPub, make_error_code(boost::system::errc::bad_message));
        return;
      }
      
      if (wire.type() == tlv::PubFromRp)
      {
        INFO("Sucessfully decoded PubFromRp packet!");
        DEBUG("Wire Type: %d\nWire Length: %lu", wire.type(), wire.value_size());
        PubFromRp pubFromRp(wire);
        recvCallback(pubFromRp, make_error_code(boost::system::errc::success));
        asyncRead();
      }
      else
      {
        INFO("Packet successfully decoded but Type is not PubFromRp");
      }
    }
  );
}

shared_ptr<UdpTransport>
UdpTransport::create(io_service& ioService)
{
  return make_shared<UdpTransport>(ioService);
}

}   // namespace client
}   // namespace fcopss
