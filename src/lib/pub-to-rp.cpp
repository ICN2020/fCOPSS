/*
  pub-to-rp.cpp

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

#include <fcopss/pub-to-rp.hpp>

namespace fcopss {

PubToRp::PubToRp()
{
}

PubToRp::PubToRp(const Cd& cd)
  : Pub(cd)
{
}

PubToRp::PubToRp(const Block& wire)
{
  wireDecode(wire);
}

PubToRp::~PubToRp()
{
}

template<ndn::encoding::Tag TAG>
size_t
PubToRp::wireEncode(ndn::encoding::EncodingImpl<TAG>& encoder) const
{
  size_t totalLength = 0;

  totalLength += encoder.prependBlock(getContent());
  totalLength += m_cd.wireEncode(encoder);
  totalLength += encoder.prependVarNumber(totalLength);
  totalLength += encoder.prependVarNumber(tlv::PubToRp);

  return totalLength;
}

template size_t
PubToRp::wireEncode<ndn::encoding::EncoderTag>(ndn::encoding::EncodingImpl<ndn::encoding::EncoderTag>& encoder) const;

template size_t
PubToRp::wireEncode<ndn::encoding::EstimatorTag>(ndn::encoding::EncodingImpl<ndn::encoding::EstimatorTag>& encoder) const;

const Block&
PubToRp::wireEncode() const
{
  if (m_wire.hasWire()) {
    return m_wire;
  }

  ndn::encoding::EncodingEstimator estimator;
  size_t estimatedSize = wireEncode(estimator);

  ndn::encoding::EncodingBuffer buffer(estimatedSize, 0);
  wireEncode(buffer);

  const_cast<PubToRp*>(this)->wireDecode(buffer.block());

  return m_wire;
}

void
PubToRp::wireDecode(const Block& wire)
{
  m_wire = wire;
  m_wire.parse();

  if (m_wire.type() != tlv::PubToRp) {
    BOOST_THROW_EXCEPTION(Error("Unexpected TLV type when decoding PubToRp"));
  }

  m_cd.wireDecode(m_wire.get(tlv::Cd));
  m_content = m_wire.get(tlv::Content);
}

PubToRp&
PubToRp::setCd(const Cd& cd)
{
  Pub::setCd(cd);
  return *this;
}

PubToRp&
PubToRp::setContent(const uint8_t* buffer, size_t bufferSize)
{
  Pub::setContent(buffer, bufferSize);
  return *this;
}

PubToRp&
PubToRp::setContent(const Block& block)
{
  Pub::setContent(block);
  return *this;
}

bool
PubToRp::operator==(const PubToRp& other) const
{
  return Pub::operator==(other);
}

bool
PubToRp::operator!=(const PubToRp& other) const
{
  return !(*this == other);
}

ostream&
operator<<(ostream& os, const PubToRp& pub)
{
  os << pub.getCd();

  return os;
}

} // namespace fcopss
