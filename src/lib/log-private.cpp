/*
  log-private.cpp

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
#include <fcopss/log-private.hpp>
#include "sys/time.h"

static const int BUFSIZE = 512;

namespace fcopss {

Log::Log()
  : m_configLogLvl(LogLevel::LOG_INFO)
  , m_filename("")
{
  char* fcopss_log_file = getenv("FCOPSS_LOG_FILE");
  char* fcopss_log_level = getenv("FCOPSS_LOG_LEVEL");
  
  if (fcopss_log_level)
  {
    if (strcmp(fcopss_log_level, "ERROR") == 0)
      m_configLogLvl = fcopss::LogLevel::LOG_ERROR;
    else if (strcmp(fcopss_log_level, "WARN") == 0)
      m_configLogLvl = fcopss::LogLevel::LOG_WARN;
    else if (strcmp(fcopss_log_level, "INFO") == 0)
      m_configLogLvl = fcopss::LogLevel::LOG_INFO;
    else if (strcmp(fcopss_log_level, "DEBUG") == 0)
      m_configLogLvl = fcopss::LogLevel::LOG_DEBUG;
    else if (strcmp(fcopss_log_level, "NONE") == 0)
      m_configLogLvl = fcopss::LogLevel::LOG_NONE;
  }

  if (fcopss_log_file)
    m_filename = static_cast<string>(fcopss_log_file);

  if (!m_filename.empty())  // print to given filename
  {
    m_outStream.open(m_filename.c_str());

    if (!m_outStream.is_open())
    {
      throw(std::runtime_error("LOGGER: unable to open output stream"));
    }
  }
}

Log::~Log()
{
  if (m_outStream.is_open())
  {
    m_outStream.close();
  }
}

Log& 
Log::getInstance()
{
  static Log globalLog;
  return globalLog;
}

string 
Log::getTime()
{
  timespec now;
  time_t nowS;
  tm *nowLT;
  char tempBuffer[20];
  char timeBuffer[30];  // format has 29 chars + 1 termination null char

  clock_gettime(CLOCK_REALTIME, &now);
  nowS = now.tv_sec;
  nowLT = localtime(&nowS);

  strftime(tempBuffer, sizeof tempBuffer, "%F %T", nowLT);
  snprintf(timeBuffer, sizeof timeBuffer, "%s.%09ld", tempBuffer, now.tv_nsec);

  return static_cast<string>(timeBuffer);
}

string 
Log::getLevel(LogLevel lvl)
{
  string logLevel = "";
  switch(lvl)
  {
    case LogLevel::LOG_ERROR:
      logLevel = " [ERROR]";
      break;

    case LogLevel::LOG_WARN:
      logLevel = " [WARN]";
      break;

    case LogLevel::LOG_INFO:
      logLevel = " [INFO]";
      break;

    case LogLevel::LOG_DEBUG:
      logLevel = " [DEBUG]";
      break;

    case LogLevel::LOG_NONE:
      break;
  }
  
  return logLevel;
}

void 
Log::print(LogLevel lvl, const char* format, ...)
{
  if (lvl > m_configLogLvl)
  {
    return;
  }

  va_list args;
  va_start(args, format);
  printImpl(lvl, format, args);
  va_end(args);
}

void 
Log::printImpl(LogLevel lvl, const char* format, va_list args)
{
  char buffer[BUFSIZE];
  
  if (vsnprintf(buffer, sizeof buffer, format, args) < 0)
  {
    buffer[BUFSIZE - 1] = '\0';
  }

  string s;
  stringstream ss(buffer);
  
  bool isFirstLoop = true;
  string time = getTime();
  string level = getLevel(lvl);
  
  std::lock_guard< std::mutex> guard(m_writeMutex);

  do
  {
    getline(ss, s);
    m_logStream << time << level;

    if (isFirstLoop)
    {
      m_logStream << s;
    }
    else
    {
      string tempBuffer = buffer;
      unsigned open_delim_pos = tempBuffer.find_first_of("[");
      unsigned close_delim_pos = tempBuffer.find_first_of("]", open_delim_pos);
      m_logStream << tempBuffer.substr(open_delim_pos, close_delim_pos - open_delim_pos + 2);
      open_delim_pos = tempBuffer.find_first_of("[", close_delim_pos);
      close_delim_pos = tempBuffer.find_first_of("]", open_delim_pos);
      m_logStream << tempBuffer.substr(open_delim_pos, close_delim_pos - open_delim_pos + 2);
      m_logStream << s;
    }
  
    if (!ss.eof())
    {
      m_logStream << "\n";
    }

    isFirstLoop = false;
  } while (!ss.eof());

  if (!m_filename.empty())  // if filename was given
  {
    m_outStream << m_logStream.str() << std::endl;
  }
  else
  {
    std::cout << m_logStream.str() << std::endl;
  }

  m_logStream.str("");
}

} // namespace fcopss
