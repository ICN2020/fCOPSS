/*
  common.hpp

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
#ifndef _FCOPSS_COMMON_HPP_
#define _FCOPSS_COMMON_HPP_

#include <cstddef>
#include <cstdint>
#include <limits>
#include <memory>
#include <functional>
#include <tuple>
#include <string>
#include <vector>
#include <list>
#include <set>
#include <iostream>
#include <fstream>
#include <sstream>
#include <stdexcept>

#include <boost/asio.hpp>
#include <boost/noncopyable.hpp>

#include <ndn-cxx/common.hpp>
#include <ndn-cxx/encoding/buffer.hpp>
#include <ndn-cxx/encoding/block.hpp>
#include <ndn-cxx/encoding/block-helpers.hpp>
#include <ndn-cxx/encoding/encoding-buffer.hpp>
#include <ndn-cxx/util/string-helper.hpp>

namespace fcopss {

using std::size_t;
using std::shared_ptr;
using std::make_shared;
using std::enable_shared_from_this;
using std::weak_ptr;
using std::unique_ptr;
using std::function;
using std::tuple;
using std::tie;
using std::string;
using std::vector;
using std::list;
using std::set;
using std::istream;
using std::ostream;
using std::ifstream;
using std::ofstream;
using std::stringstream;
using std::runtime_error;

using boost::noncopyable;
using boost::asio::io_service;
using boost::asio::ip::tcp;
using boost::asio::ip::udp;
using boost::asio::deadline_timer;
using boost::posix_time::time_duration;

using Buffer = ndn::Buffer;
using Block = ndn::Block;

const size_t MAX_PACKET_SIZE = 8192;

} // namespace fcopss

#endif // _FCOPSS_COMMON_HPP_

