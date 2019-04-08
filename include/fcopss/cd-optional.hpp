/*
  cd-optional.hpp

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
#ifndef _FCOPSS_CD_OPTIONAL_HPP_
#define _FCOPSS_CD_OPTIONAL_HPP_

#include <fcopss/common.hpp>
#include <fcopss/tlv.hpp>

namespace fcopss {

using ndn::ConstBufferPtr;

namespace cd {

class Optional : public Block
{
public:

  class Error : public Block::Error
  {
  public:
    explicit
    Error(const string& what)
      : Block::Error(what)
    {
    }
  };

  Optional();

  Optional(const Block& wire);

  explicit
  Optional(const ConstBufferPtr& buffer);

  explicit
  Optional(const Buffer& buffer);

  Optional(const uint8_t* buffer, size_t bufferSize);

  template<class Iterator>
  Optional(Iterator first, Iterator last);
 
  explicit
  Optional(const char* str);

  explicit
  Optional(const string& str);

  void
  toUri(ostream& os) const;

  string
  toUri() const;

  template<ndn::encoding::Tag TAG>
  size_t
  wireEncode(ndn::encoding::EncodingImpl<TAG>& encoder) const;

  const Block&
  wireEncode() const;

  void
  wireDecode(const Block& wire);

  bool
  empty() const;

  bool
  equals(const Optional& other) const;

  int
  compare(const Optional& other) const;

  bool
  operator==(const Optional& other) const;

  bool
  operator!=(const Optional& other) const;

  bool
  operator<=(const Optional& other) const;

  bool
  operator<(const Optional& other) const;

  bool
  operator>=(const Optional& other) const;

  bool
  operator>(const Optional& other) const;
};

inline ostream&
operator<<(ostream& os, const Optional& optional)
{
  optional.toUri(os);
  return os;
}

template<class Iterator>
inline
Optional::Optional(Iterator first, Iterator last)
  : Block(ndn::encoding::makeBinaryBlock(tlv::CdOptional, first, last))
{
}

} // namespace cd
} // namespace fcopss

#endif // _FCOPSS_CD_OPTIONAL_HPP_
