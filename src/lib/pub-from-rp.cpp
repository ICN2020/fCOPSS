/*
  pub-from-rp.cpp

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

#include <fcopss/pub-from-rp.hpp>
#include <fcopss/pub-to-rp.hpp>

namespace fcopss {

PubFromRp::PubFromRp()
{
}

PubFromRp::PubFromRp(const Cd& cd)
  : Pub(cd)
{
}

PubFromRp::PubFromRp(const Block& wire)
{
  wireDecode(wire);
}

PubFromRp::PubFromRp(const PubToRp& pub)
  : Pub(pub.getCd())
{
  Pub::setContent(pub.getContent());
}

PubFromRp::~PubFromRp()
{
}

template<ndn::encoding::Tag TAG>
size_t
PubFromRp::wireEncode(ndn::encoding::EncodingImpl<TAG>& encoder) const
{
  size_t totalLength = 0;

  totalLength += encoder.prependBlock(getContent());
  totalLength += m_cd.wireEncode(encoder);
  totalLength += encoder.prependVarNumber(totalLength);
  totalLength += encoder.prependVarNumber(tlv::PubFromRp);

  return totalLength;
}

template size_t
PubFromRp::wireEncode<ndn::encoding::EncoderTag>(ndn::encoding::EncodingImpl<ndn::encoding::EncoderTag>& encoder) const;

template size_t
PubFromRp::wireEncode<ndn::encoding::EstimatorTag>(ndn::encoding::EncodingImpl<ndn::encoding::EstimatorTag>& encoder) const;

const Block&
PubFromRp::wireEncode() const
{
  if (m_wire.hasWire()) {
    return m_wire;
  }

  ndn::encoding::EncodingEstimator estimator;
  size_t estimatedSize = wireEncode(estimator);

  ndn::encoding::EncodingBuffer buffer(estimatedSize, 0);
  wireEncode(buffer);

  const_cast<PubFromRp*>(this)->wireDecode(buffer.block());

  return m_wire;
}

void
PubFromRp::wireDecode(const Block& wire)
{
  m_wire = wire;
  m_wire.parse();

  if (m_wire.type() != tlv::PubFromRp) {
    BOOST_THROW_EXCEPTION(Error("Unexpected TLV type when decoding PubFromRp"));
  }

  m_cd.wireDecode(m_wire.get(tlv::Cd));
  m_content = m_wire.get(tlv::Content);
}

PubFromRp&
PubFromRp::setCd(const Cd& cd)
{
  Pub::setCd(cd);
  return *this;
}

PubFromRp&
PubFromRp::setContent(const uint8_t* buffer, size_t bufferSize)
{
  Pub::setContent(buffer, bufferSize);
  return *this;
}

PubFromRp&
PubFromRp::setContent(const Block& block)
{
  Pub::setContent(block);
  return *this;
}

bool
PubFromRp::operator==(const PubFromRp& other) const
{
  return Pub::operator==(other);
}

bool
PubFromRp::operator!=(const PubFromRp& other) const
{
  return !(*this == other);
}

ostream&
operator<<(ostream& os, const PubFromRp& pub)
{
  os << pub.getCd();

  return os;
}

} // namespace fcopss
