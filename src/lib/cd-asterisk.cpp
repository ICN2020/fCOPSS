/*
  cd-asterisk.cpp

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
#include <fcopss/cd-asterisk.hpp>

namespace fcopss {
namespace cd {

Asterisk::Asterisk()
  : Block(tlv::CdAsterisk)
{
}

Asterisk::Asterisk(const Block& wire)
  : Block(wire)
{
  if (type() != tlv::CdAsterisk) {
    BOOST_THROW_EXCEPTION(Error("Cannot construct cd::Asterisk from not a CdAsterisk TLV wire block"));
  }
  if (value_size() > 0) {
    BOOST_THROW_EXCEPTION(Error("Cannot construct cd::Asterisk from non zero length CdAsterisk TLV wire block"));
  }
}

Asterisk::Asterisk(const ConstBufferPtr& buffer)
  : Block(tlv::CdAsterisk, buffer)
{
  if (value_size() > 0) {
    BOOST_THROW_EXCEPTION(Error("Cannot construct cd::Asterisk from non zero length CdAsterisk TLV wire block"));
  }
}

Asterisk::Asterisk(const Buffer& value)
  : Block(ndn::encoding::makeBinaryBlock(tlv::CdAsterisk, value.data(), value.size()))
{
  if (value_size() > 0) {
    BOOST_THROW_EXCEPTION(Error("Cannot construct cd::Asterisk from non zero length CdAsterisk TLV wire block"));
  }
}

Asterisk::Asterisk(const uint8_t* value, size_t valueLen)
  : Block(ndn::encoding::makeBinaryBlock(tlv::CdAsterisk, value, valueLen))
{
  if (value_size() > 0) {
    BOOST_THROW_EXCEPTION(Error("Cannot construct cd::Asterisk from non zero length CdAsterisk TLV wire block"));
  }
}

Asterisk::Asterisk(const char* str)
  : Block(ndn::encoding::makeBinaryBlock(tlv::CdAsterisk, str, std::char_traits<char>::length(str)))
{
  if (value_size() > 0) {
    BOOST_THROW_EXCEPTION(Error("Cannot construct cd::Asterisk from non zero length CdAsterisk TLV wire block"));
  }
}

Asterisk::Asterisk(const string& str)
  : Block(ndn::encoding::makeStringBlock(tlv::CdAsterisk, str))
{
  if (value_size() > 0) {
    BOOST_THROW_EXCEPTION(Error("Cannot construct cd::Asterisk from non zero length CdAsterisk TLV wire block"));
  }
}

void
Asterisk::toUri(ostream& os) const
{
  // asterisk has no value
}

string
Asterisk::toUri() const
{
  std::ostringstream os;
  toUri(os);
  return os.str();
}

template<ndn::encoding::Tag TAG>
size_t
Asterisk::wireEncode(ndn::encoding::EncodingImpl<TAG>& encoder) const
{
  size_t totalLength = 0;
  if (value_size() > 0)
    totalLength += encoder.prependByteArray(value(), value_size());
  totalLength += encoder.prependVarNumber(value_size());
  totalLength += encoder.prependVarNumber(type());
  return totalLength;
}

template size_t
Asterisk::wireEncode<ndn::encoding::EncoderTag>(ndn::encoding::EncodingImpl<ndn::encoding::EncoderTag>& encoder) const;

template size_t
Asterisk::wireEncode<ndn::encoding::EstimatorTag>(ndn::encoding::EncodingImpl<ndn::encoding::EstimatorTag>& encoder) const;

const Block&
Asterisk::wireEncode() const
{
  if (this->hasWire())
    return *this;

  ndn::encoding::EncodingEstimator estimator;
  size_t estimatedSize = wireEncode(estimator);

  ndn::encoding::EncodingBuffer buffer(estimatedSize, 0);
  wireEncode(buffer);

  const_cast<Asterisk&>(*this) = buffer.block();
  return *this;
}

void
Asterisk::wireDecode(const Block& wire)
{
  *this = wire;
  // validity check is done within Asterisk(const Block& wire)
}

bool
Asterisk::empty() const
{
  return (value_size() == 0);
}

bool
Asterisk::equals(const Asterisk& other) const
{
  return type() == other.type() &&
         value_size() == other.value_size() &&
         (empty() || // needed on OSX 10.9 due to STL bug
          std::equal(value_begin(), value_end(), other.value_begin()));
}

int
Asterisk::compare(const Asterisk& other) const
{
  if (this->hasWire() && other.hasWire()) {
    // In the common case where both components have wire encoding,
    // it's more efficient to simply compare the wire encoding.
    // This works because lexical order of TLV encoding happens to be
    // the same as canonical order of the value.
    return std::memcmp(wire(), other.wire(), std::min(size(), other.size()));
  }

  int cmpType = type() - other.type();
  if (cmpType != 0)
    return cmpType;

  int cmpSize = value_size() - other.value_size();
  if (cmpSize != 0)
    return cmpSize;

  if (empty()) // needed on OSX 10.9 due to STL bug
    return 0;

  return std::memcmp(value(), other.value(), value_size());
}

bool
Asterisk::operator==(const Asterisk& other) const
{
  return equals(other);
}

bool
Asterisk::operator!=(const Asterisk& other) const
{
  return !equals(other);
}

bool
Asterisk::operator<=(const Asterisk& other) const
{
  return compare(other) <= 0;
}

bool
Asterisk::operator<(const Asterisk& other) const
{
  return compare(other) < 0;
}

bool
Asterisk::operator>=(const Asterisk& other) const
{
  return compare(other) >= 0;
}

bool
Asterisk::operator>(const Asterisk& other) const
{
  return compare(other) > 0;
}

} // namespace cd
} // namespace fcopss

