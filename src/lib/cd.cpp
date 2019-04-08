/*
  cd.cpp

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

#include <boost/algorithm/string.hpp>

#include <fcopss/cd.hpp>

namespace fcopss {

const size_t Cd::npos = std::numeric_limits<size_t>::max();

Cd::Cd()
  : m_cdBlock(tlv::Cd)
{
}

Cd::Cd(const Block& wire)
{
  m_cdBlock = wire;
  m_cdBlock.parse();
}

Cd::Cd(const char* uri)
  : Cd(string(uri))
{
}

Cd::Cd(const string& uri)
  : m_cdBlock(tlv::Cd)
{
  string s = uri;

  boost::trim_if(s, boost::is_any_of("/"));

  if (s.empty()) {
    // special case: uri is empty or slash only
    return;
  }

  vector<string> words;
  boost::split(words, s, boost::is_any_of("/"));

  parseCdString(words, m_cdBlock);
  m_cdBlock.parse();
}

void
Cd::parseCdString(vector<string>& words, Block& block)
{
  State state = NotOpened;

  for (auto word : words) {
    Event event = getEventFromString(word);

    switch (state) {
    case NotOpened:
      if (event == Open) {
        block.push_back(cd::Component(word));
        state = Opened;
      } else if (event == Close) {
        BOOST_THROW_EXCEPTION(Error("CD parse error: illeagal ')' in name component"));
      } else {
        block.push_back(cd::Component(word));
      }
      break;
    case Opened:
      if (event == Open) {
        BOOST_THROW_EXCEPTION(Error("CD parse error: illeagal '(' in name component"));
      } else if (event == Close) {
        block.push_back(cd::Optional(word));
        block.push_back(cd::Asterisk());
        state = Closed;
      } else {
        block.push_back(cd::Optional(word));
      }
      break;
    case Closed:
      if (event == Open) {
        BOOST_THROW_EXCEPTION(Error("CD parse error: illeagal '(' in name component"));
      } else if (event == Close) {
        BOOST_THROW_EXCEPTION(Error("CD parse error: illeagal ')' in name component"));
      } else {
        block.push_back(cd::Component(word));
      }
      break;
    }
  }
  size_t size = block.elements().size();
  if (size > 0) {
    if (block.elements()[size - 1].type() != tlv::CdComponent) {
        BOOST_THROW_EXCEPTION(Error("CD parse error: illeagal last name component"));
    }
  }
}

Cd::Event
Cd::getEventFromString(string& word)
{
    if (word.empty()) {
        BOOST_THROW_EXCEPTION(Error("CD parse error: empty name component"));
    }

    if (word.back() == '(') {
      word.erase(word.end() - 1);
      if (boost::contains(word, "(")) {
        BOOST_THROW_EXCEPTION(Error("CD parse error: more than one '(' in name component"));
      }
      return Open;
    }

    if (word.back() == ')') {
      word.erase(word.end() - 1);
      if (boost::contains(word, ")")) {
        BOOST_THROW_EXCEPTION(Error("CD parse error: more than one ')' in name component"));
      }
      return Close;
    }

    if (boost::contains(word, "(")) {
        BOOST_THROW_EXCEPTION(Error("CD parse error: illeagal '(' in name component"));
    }
    if (boost::contains(word, ")")) {
        BOOST_THROW_EXCEPTION(Error("CD parse error: illeagal ')' in name component"));
    }
    
    return Hold;
}

string
Cd::toUri() const
{
  std::ostringstream os;
  os << *this;
  return os.str();
}

template<ndn::encoding::Tag TAG>
size_t
Cd::wireEncode(ndn::encoding::EncodingImpl<TAG>& encoder) const
{
  size_t totalLength = 0;

  Block::element_container::const_reverse_iterator it = m_cdBlock.elements().rbegin();
  while (it != m_cdBlock.elements().rend()) {
    if (it->type() == tlv::CdOptional) {
      const cd::Optional* p = reinterpret_cast<const cd::Optional*>(&*it);
      totalLength += p->wireEncode(encoder);
    } else if (it->type() == tlv::CdAsterisk) {
      const cd::Asterisk* p = reinterpret_cast<const cd::Asterisk*>(&*it);
      totalLength += p->wireEncode(encoder);
    } else {
      const cd::Component* p = reinterpret_cast<const cd::Component*>(&*it);
      totalLength += p->wireEncode(encoder);
    }
    it++;
  }

  totalLength += encoder.prependVarNumber(totalLength);
  totalLength += encoder.prependVarNumber(tlv::Cd);
  return totalLength;
}

template size_t
Cd::wireEncode<ndn::encoding::EncoderTag>(ndn::encoding::EncodingImpl<ndn::encoding::EncoderTag>& encoder) const;

template size_t
Cd::wireEncode<ndn::encoding::EstimatorTag>(ndn::encoding::EncodingImpl<ndn::encoding::EstimatorTag>& encoder) const;

const Block&
Cd::wireEncode() const
{
  if (m_cdBlock.hasWire())
    return m_cdBlock;

  ndn::encoding::EncodingEstimator estimator;
  size_t estimatedSize = wireEncode(estimator);

  ndn::encoding::EncodingBuffer buffer(estimatedSize, 0);
  wireEncode(buffer);

  m_cdBlock = buffer.block();
  m_cdBlock.parse();

  return m_cdBlock;
}

void
Cd::wireDecode(const Block& wire)
{
  if (wire.type() != tlv::Cd) {
    BOOST_THROW_EXCEPTION(Error("Unexpected TLV type when decoding Cd"));
  }

  m_cdBlock = wire;
  m_cdBlock.parse();
}

bool
Cd::hasWire() const
{
  return m_cdBlock.hasWire();
}

bool
Cd::equals(const Cd& cd) const
{
  if (size() != cd.size()) {
    return false;
  }

  for (size_t i = 0; i < size(); i++) {
    if (at(i) != cd.at(i)) {
      return false;
    }
  }

  return true;
}

bool
Cd::isPrefixOf(const Cd& cd) const
{
  if (size() > cd.size()) {
    return false;
  }

  for (size_t i = 0; i < size(); i++) {
    if (at(i) != cd.at(i)) {
      return false;
    }
  }

  return true;
}

bool
Cd::empty() const
{
  return m_cdBlock.elements().empty();
}

size_t
Cd::size() const
{
  return m_cdBlock.elements().size();
}

const Block::element_container&
Cd::elements() const
{
  return m_cdBlock.elements();
}

const Block&
Cd::get(ssize_t i) const
{
  if (i < 0) {
    i = size() + i;
  }
  return m_cdBlock.elements()[i];
}

const Block&
Cd::operator[](ssize_t i) const
{
  return get(i);
}

const Block&
Cd::at(ssize_t i) const
{
  ssize_t min = -size();
  ssize_t max = size() - 1;

  if ((i < min) || (max < i)) {
    BOOST_THROW_EXCEPTION(Error("CD conmponent index out of range"));
  }

  return get(i);
}

int
Cd::compare(const Cd& other) const
{
  return compare(0, npos, other);
}

int
Cd::compare(size_t pos1, size_t count1, const Cd& other, size_t pos2, size_t count2) const
{
  count1 = std::min(count1, size() - pos1);
  count2 = std::min(count2, other.size() - pos2);
  size_t count = std::min(count1, count2);

  for (size_t i = 0; i < count; ++i) {
    int comp = compare(at(pos1 + i), other.at(pos2 + i));
    if (comp != 0) { // i-th component differs
      return comp;
    }
  }
  // [pos1, pos1+count) of this Name equals [pos2, pos2+count) of other Name
  return count1 - count2;
}

int
Cd::compare(const Block& b1, const Block& b2)
{
  if (b1.hasWire() && b2.hasWire()) {
    return std::memcmp(b1.wire(), b2.wire(), std::min(b1.size(), b2.size()));
  }

  int cmpType = b1.type() - b2.type();
  if (cmpType != 0)
    return cmpType;

  int cmpSize = b1.value_size() - b2.value_size();
  if (cmpSize != 0)
    return cmpSize;

  if (b1.empty())
    return 0;

  return std::memcmp(b1.value(), b2.value(), b1.value_size());
}

bool
Cd::operator==(const Cd& other) const
{
  return equals(other);
}

bool
Cd::operator!=(const Cd& other) const
{
  return !equals(other);
}

bool
Cd::operator<=(const Cd& other) const
{
  return compare(other) <= 0;
}

bool
Cd::operator<(const Cd& other) const
{
  return compare(other) < 0;
}

bool
Cd::operator>=(const Cd& other) const
{
  return compare(other) >= 0;
}

bool
Cd::operator>(const Cd& other) const
{
  return compare(other) > 0;
}

ostream&
operator<<(ostream& os, const Cd& cd)
{
  if (cd.empty()) {
    os << "/";
  } else {
    bool first = true;
    Block::element_container::const_iterator it = cd.elements().begin();
    while (it != cd.elements().end()) {
      if (it->type() == tlv::CdOptional) {
        if (first) {
          os << "(";
          first = false;
        }
        os << "/";
        const cd::Optional* p = reinterpret_cast<const cd::Optional*>(&*it);
        p->toUri(os);
      } else if (it->type() == tlv::CdAsterisk) {
        const cd::Asterisk* p = reinterpret_cast<const cd::Asterisk*>(&*it);
        os << ")";
        p->toUri(os);
      } else {
        os << "/";
        const cd::Component* p = reinterpret_cast<const cd::Component*>(&*it);
        p->toUri(os);
      }
      it++;
    }
  }
  return os;
}

istream&
operator>>(istream& is, Cd& cd)
{
  string uri;
  is >> uri;
  cd = Cd(uri);

  return is;
}

} // namespace fcopss
