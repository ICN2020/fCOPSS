/*
  log-private.hpp

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
#ifndef _FCOPSS_LOG_PRIVATE_HPP_
#define _FCOPSS_LOG_PRIVATE_HPP_

#include <fcopss/common.hpp>
#include <cstdio>
#include <cstdarg>
#include <mutex>

namespace fcopss {

enum LogLevel {
  LOG_NONE = 0,
  LOG_ERROR = 1,
  LOG_WARN = 2,
  LOG_INFO = 3,
  LOG_DEBUG = 4
};

class Log : noncopyable
{
public:

  static Log& 
  getInstance();
  
  void 
  print(LogLevel lvl, const char* format, ...);

private:
  ofstream      m_outStream;
  stringstream  m_logStream;
  std::mutex    m_writeMutex;
  LogLevel      m_configLogLvl;
  string        m_filename;

  Log();
  
  ~Log();
  
  string 
  getTime();

  string 
  getLevel(LogLevel lvl);
  
  void 
  printImpl(LogLevel lvl, const char* format, va_list args);
};

} //namespace fcopss

#endif
