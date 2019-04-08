/*
  tcp-transport.cpp

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
#include <fcopss/tcp-transport.hpp>
#include <fcopss/config.hpp>
#include <fcopss/tlv.hpp>
#include <fcopss/log.hpp>

namespace fcopss {
namespace client {

using namespace std::placeholders;

TcpTransport::TcpTransport(io_service& ioService)
  : Transport(ioService)
  , m_socket(ioService)
  , m_deadlineTimer(ioService)
  , m_isTimerStarted(false)
  , m_inputBufferSize(0)
  , m_isConnected(-1)   // -1: not connected
  , m_isReadFromConnectHandler(false)
  , m_resolver(ioService)
{
  Config config;
  stringstream portNumber;
  portNumber << config.routerPort();
  m_routerPort = portNumber.str();
  m_tcpReceiveTimeout = config.tcpReceiveTimeout().total_seconds();
  m_tcpConnectionTimeout = config.tcpConnectionTimeout().total_seconds();
}

TcpTransport::~TcpTransport()
{
  close();
}

void
TcpTransport::connect()
{
  INFO("Initiating socket connection");
  tcp::resolver::query q("localhost", m_routerPort);  // connect to localhost at the specified port 
  
  // m_deadlineTimer only runs either during connection OR during sending.
  // there will not be a situation where 2 timers are set concurrently
  m_deadlineTimer.expires_from_now(boost::posix_time::seconds(m_tcpConnectionTimeout));
  m_deadlineTimer.async_wait(bind(&TcpTransport::connectTimeoutHandler, shared_from_this(), _1));
  m_isTimerStarted = true;
  m_resolver.async_resolve(q, bind(&TcpTransport::resolveHandler, shared_from_this(), _1, _2));
  m_isConnected = 0;  // 0: connecting
}

void
TcpTransport::connectTimeoutHandler(const boost::system::error_code& error)
{
  if (error)
  {
    if (error == boost::system::errc::operation_canceled) // operation explicitly cancelled
    {
      WARN("async_wait (tcp connection) operation explicitly canceled");
      return;
    }
  
    close();
    ERROR("Error in async_wait for tcp connection");
    BOOST_THROW_EXCEPTION(Error(error, "Error in async_wait for tcp connection"));
  }

  close();
  WARN("Tcp connection timed out");
  BOOST_THROW_EXCEPTION(Error("Tcp connection timed out"));
}

void
TcpTransport::resolveHandler(const boost::system::error_code& error, tcp::resolver::iterator i) 
{
  if (error)  
  {
    if (error == boost::system::errc::operation_canceled) // operation explicitly cancelled
    {
      WARN("async_resolve operation explicitly canceled");
      return;
    }
    
    close();
    ERROR("Error while resolving query");
    BOOST_THROW_EXCEPTION(Error(error, "Error while resolving query"));
  }

  boost::asio::async_connect(m_socket, i, bind(&TcpTransport::connectHandler, shared_from_this(), _1, _2));
}

void
TcpTransport::connectHandler(const boost::system::error_code& error, tcp::resolver::iterator i) 
{
  if (error)  
  {
    if (error == boost::system::errc::operation_canceled) // operation explicitly cancelled
    {
      WARN("async_connect operation explicitly canceled");
      return;
    }
    
    close();
    ERROR("Error while connecting to router(forwarder)");
    BOOST_THROW_EXCEPTION(Error(error, "Error while connecting to router(forwarder)"));
  }
  
  m_deadlineTimer.cancel();
  m_isTimerStarted = false;
  m_isConnected = 1;  // 1: connected
  DEBUG("Connected to %s at port %hu from %s at port %hu", 
       m_socket.remote_endpoint().address().to_string().c_str(), m_socket.remote_endpoint().port(),
       m_socket.local_endpoint().address().to_string().c_str(), m_socket.local_endpoint().port());
  
  if (m_isReadFromConnectHandler)
  {
    receive();
  }

  if (!m_transmissionQueue.empty())
  {
    send();
  }

}
void
TcpTransport::ensureConnected()
{
  if (!m_socket.is_open() && m_isConnected != 0)    // not open and not in the process of connecting
  {
    m_isConnected = -1;   // -1: not connected
    connect();
  }
}

void
TcpTransport::close()
{
  if (m_isTimerStarted)   // cancel timer
  {
    INFO("Canceling Timeout timer...");
    m_deadlineTimer.cancel();  
    m_isTimerStarted = false;
  }

  if (m_socket.is_open())
  {
    INFO("Closing socket...");
    m_socket.shutdown(tcp::socket::shutdown_send);
    m_socket.close();
    m_isConnected = -1;   // -1: not connected
  }
}

void
TcpTransport::asyncWrite(const PubToRp& pub)
{
  m_transmissionQueue.push_back(pub.wireEncode());
  ensureConnected(); 
  if (m_isConnected > 0 && m_transmissionQueue.size() == 1)
  {
    send();
  }

  // if not connected/connecting (m_isConnected <= 0) or 
  // transmission is in progress (m_transmissionQueue > 1),
  // write will be carried out by connectHandler or asyncWriteHandler
}

void
TcpTransport::asyncWrite(const Sub& sub)
{
  m_transmissionQueue.push_back(sub.wireEncode());
  ensureConnected();
  if (m_isConnected > 0 && m_transmissionQueue.size() == 1)
  {
    send();
  }

  // if not connected/connecting (m_isConnected <= 0) or 
  // transmission is in progress (m_transmissionQueue > 1),
  // write will be carried out by connectHandler or asyncWriteHandler
}

void 
TcpTransport::send() 
{
  const Block& wire = m_transmissionQueue.front();
  shared_ptr<TcpTransport> self(shared_from_this());
  
  INFO("Sending packet...");
  DEBUG("Wire Type: %d\nWire Length: %lu", wire.type(), wire.value_size());
  
  boost::asio::async_write(
    m_socket,
    boost::asio::buffer(wire.wire(), wire.size()),
    [this, self] (const boost::system::error_code& error, size_t bytesTransferred) {
      if (error)  
      {
        if (error == boost::system::errc::operation_canceled) // operation explicitly cancelled
        {
          WARN("async_write (operation explicitly canceled");
          return;
        }

        close();
        ERROR("Error while sending to router (forwarder)");
        BOOST_THROW_EXCEPTION(Error(error, "Error while sending to router (forwarder)"));
      }
      
      m_transmissionQueue.pop_front();
      INFO("Packet (%ld bytes) Sent!", bytesTransferred);

      if (!m_transmissionQueue.empty())
      {
        send();
      }
    }
  );
}

void
TcpTransport::asyncRead()
{
  ensureConnected();
  
  if (m_isConnected == 1)
  {
    receive();
  }
  else
  {
    m_isReadFromConnectHandler = true; 
  }

  // if it is not yet connected, receive will be called by the connectHandler instead
}

void 
TcpTransport::receive()
{
  shared_ptr<TcpTransport> self(shared_from_this());
  
  INFO("Listening for PubFromRp messages...");
  m_socket.async_receive(
    boost::asio::buffer(m_inputBuffer + m_inputBufferSize, MAX_PACKET_SIZE - m_inputBufferSize), 
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

      m_inputBufferSize += bytesReceived;
      size_t offset = 0;
      bool hasFullMessage = processMessage(offset);

      if (!hasFullMessage && m_inputBufferSize == MAX_PACKET_SIZE && offset == 0)   // if 2 message overflows the buffer, its ok
      {
        close();
        ERROR("Buffer is full but failed to decode received message");
        recvCallback(emptyPub, make_error_code(boost::system::errc::bad_message));
        return;
      }
    
      if (offset > 0)
      {
        if (offset != m_inputBufferSize)
        {
          std::copy(m_inputBuffer + offset, m_inputBuffer + m_inputBufferSize, m_inputBuffer);
          m_inputBufferSize -= offset;
        }
        else
        {
          m_inputBufferSize = 0;
        }
      }
      
      asyncRead();
    }
  );
}

bool
TcpTransport::processMessage(size_t& offset)
{
  shared_ptr<TcpTransport> self(shared_from_this());
  
  while (offset < m_inputBufferSize)
  {
    bool isSuccess = false;
    Block element;
    tie(isSuccess, element) = Block::fromBuffer(m_inputBuffer + offset, m_inputBufferSize - offset);
      
    if (!isSuccess)
    {
      INFO("Packet received is incomplete/incorrect");

      if (!m_isTimerStarted)
      {
        INFO("Starting tcpReceiveTimeout timer (%ld)...", m_tcpReceiveTimeout);
        // start the timeout timer when we have an incomplete message
        m_deadlineTimer.expires_from_now(boost::posix_time::seconds(m_tcpReceiveTimeout));
        m_deadlineTimer.async_wait(
          [this, self] (const boost::system::error_code& error) {
            PubFromRp emptyPub;
            if (error)
            {
              if (error == boost::system::errc::operation_canceled)   // operation explicitly cancelled
              {
                WARN("async_wait (tcp receive timeout) operation explicitly canceled");
                recvCallback(emptyPub, error);
                return;
              }

              close();
              ERROR("Error in async_wait for next tcp packet to complete TLV");
              recvCallback(emptyPub, error);
              return;
            }
            
            WARN("Timeout in async_wait for next tcp packet to complete TLV");
            m_isTimerStarted = false; // to restart the timeout again
            recvCallback(emptyPub, make_error_code(boost::system::errc::timed_out));
          }
        );

        m_isTimerStarted = true;
      }
      
      return false;
    }

    if (element.type() == tlv::PubFromRp)
    {
      INFO("Sucessfully decoded PubFromRp packet!");
      DEBUG("Wire Type: %d\nWire Length: %lu", element.type(), element.value_size());
      PubFromRp pubFromRp(element);
      recvCallback(pubFromRp, make_error_code(boost::system::errc::success));
    }
    else
    {
      INFO("Packet successfully decoded but Type is not PubFromRp");
    }

    offset += element.size();
  }

  if (m_isTimerStarted)
  {
    INFO("Canceling tcpReceiveTimeout timer...");
    m_deadlineTimer.cancel();   // cancel timer when we have a full message
    m_isTimerStarted = false;
  }

  return true;
}

shared_ptr<TcpTransport>
TcpTransport::create(io_service& ioService)
{
  return make_shared<TcpTransport>(ioService);
}

}   // namespace client
}   // namespace fcopss
