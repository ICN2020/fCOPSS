/*
  pub.cpp

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
#include <sstream>

#include <fcopss/pub.hpp>

namespace fcopss {

Pub::Pub()
  : m_content(tlv::Content)
{
}

Pub::Pub(const Cd& cd)
  : m_cd(cd), m_content(tlv::Content)
{
}

Pub::~Pub()
{
}

bool
Pub::hasWire() const
{
  return m_wire.hasWire();
}

const Cd&
Pub::getCd() const
{
  return m_cd;
}

Pub&
Pub::setCd(const Cd& cd)
{
  m_wire.reset();

  m_cd = cd;

  return *this;
}

const Block&
Pub::getContent() const
{
  if (m_content.empty()) {
    m_content = ndn::encoding::makeEmptyBlock(tlv::Content);
  }

  if (!m_content.hasWire()) {
    m_content.encode();
  }

  return m_content;
}

Pub&
Pub::setContent(const uint8_t* buffer, size_t bufferSize)
{
  m_wire.reset();

  m_content = ndn::encoding::makeBinaryBlock(tlv::Content, buffer, bufferSize);

  return *this;
}

Pub&
Pub::setContent(const Block& block)
{
  m_wire.reset();
  
  if (block.type() == tlv::Content) {
    m_content = block;
  } else {
    m_content = Block(tlv::Content, block);
  }

  return *this;
}

bool
Pub::operator==(const Pub& other) const
{
  return (getCd() == other.getCd()) && (getContent() == other.getContent());
}

bool
Pub::operator!=(const Pub& other) const
{
  return !(*this == other);
}

ostream&
operator<<(ostream& os, const Pub& pub)
{
  os << pub.getCd();

  return os;
}

} // namespace fcopss
