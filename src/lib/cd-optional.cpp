/*
  cd-optional.cpp

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
#include <fcopss/cd-optional.hpp>

namespace fcopss {
namespace cd {

Optional::Optional()
  : Block(tlv::CdOptional)
{
}

Optional::Optional(const Block& wire)
  : Block(wire)
{
  if (type() != tlv::CdOptional) {
    BOOST_THROW_EXCEPTION(Error("Cannot construct cd::Optional from not a CdOptional TLV wire block"));
  }
}

Optional::Optional(const ConstBufferPtr& buffer)
  : Block(tlv::CdOptional, buffer)
{
}

Optional::Optional(const Buffer& value)
  : Block(ndn::encoding::makeBinaryBlock(tlv::CdOptional, value.data(), value.size()))
{
}

Optional::Optional(const uint8_t* value, size_t valueLen)
  : Block(ndn::encoding::makeBinaryBlock(tlv::CdOptional, value, valueLen))
{
}

Optional::Optional(const char* str)
  : Block(ndn::encoding::makeBinaryBlock(tlv::CdOptional, str, std::char_traits<char>::length(str)))
{
}

Optional::Optional(const string& str)
  : Block(ndn::encoding::makeStringBlock(tlv::CdOptional, str))
{
}

void
Optional::toUri(ostream& os) const
{
  const uint8_t* value = this->value();
  size_t valueSize = value_size();

  bool gotNonDot = false;
  for (size_t i = 0; i < valueSize; ++i) {
    if (value[i] != 0x2e) {
      gotNonDot = true;
      break;
    }
  }
  if (!gotNonDot) {
    // Special case for component of zero or more periods.  Add 3 periods.
    os << "...";
    for (size_t i = 0; i < valueSize; ++i)
      os << '.';
  }
  else {
    // In case we need to escape, set to upper case hex and save the previous flags.
    std::ios::fmtflags saveFlags = os.flags(std::ios::hex | std::ios::uppercase);

    for (size_t i = 0; i < valueSize; ++i) {
      uint8_t x = value[i];
      // Check for 0-9, A-Z, a-z, (+), (-), (.), (_)
      if ((x >= 0x30 && x <= 0x39) || (x >= 0x41 && x <= 0x5a) ||
          (x >= 0x61 && x <= 0x7a) || x == 0x2b || x == 0x2d ||
          x == 0x2e || x == 0x5f)
        os << x;
      else {
        os << '%';
        if (x < 16)
          os << '0';
        os << static_cast<uint32_t>(x);
      }
    }

    // Restore.
    os.flags(saveFlags);
  }
}

string
Optional::toUri() const
{
  std::ostringstream os;
  toUri(os);
  return os.str();
}

template<ndn::encoding::Tag TAG>
size_t
Optional::wireEncode(ndn::encoding::EncodingImpl<TAG>& encoder) const
{
  size_t totalLength = 0;
  if (value_size() > 0)
    totalLength += encoder.prependByteArray(value(), value_size());
  totalLength += encoder.prependVarNumber(value_size());
  totalLength += encoder.prependVarNumber(type());
  return totalLength;
}

template size_t
Optional::wireEncode<ndn::encoding::EncoderTag>(ndn::encoding::EncodingImpl<ndn::encoding::EncoderTag>& encoder) const;

template size_t
Optional::wireEncode<ndn::encoding::EstimatorTag>(ndn::encoding::EncodingImpl<ndn::encoding::EstimatorTag>& encoder) const;

const Block&
Optional::wireEncode() const
{
  if (this->hasWire())
    return *this;

  ndn::encoding::EncodingEstimator estimator;
  size_t estimatedSize = wireEncode(estimator);

  ndn::encoding::EncodingBuffer buffer(estimatedSize, 0);
  wireEncode(buffer);

  const_cast<Optional&>(*this) = buffer.block();
  return *this;
}

void
Optional::wireDecode(const Block& wire)
{
  *this = wire;
  // validity check is done within Optional(const Block& wire)
}

bool
Optional::empty() const
{
  return (value_size() == 0);
}

bool
Optional::equals(const Optional& other) const
{
  return type() == other.type() &&
         value_size() == other.value_size() &&
         (empty() || // needed on OSX 10.9 due to STL bug
          std::equal(value_begin(), value_end(), other.value_begin()));
}

int
Optional::compare(const Optional& other) const
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
Optional::operator==(const Optional& other) const
{
  return equals(other);
}

bool
Optional::operator!=(const Optional& other) const
{
  return !equals(other);
}

bool
Optional::operator<=(const Optional& other) const
{
  return compare(other) <= 0;
}

bool
Optional::operator<(const Optional& other) const
{
  return compare(other) < 0;
}

bool
Optional::operator>=(const Optional& other) const
{
  return compare(other) >= 0;
}

bool
Optional::operator>(const Optional& other) const
{
  return compare(other) > 0;
}

} // namespace cd
} // namespace fcopss

