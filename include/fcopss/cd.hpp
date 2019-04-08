/*
  cd.hpp

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
#ifndef _FCOPSS_CD_HPP_
#define _FCOPSS_CD_HPP_

#include <fcopss/common.hpp>
#include <fcopss/cd-component.hpp>
#include <fcopss/cd-optional.hpp>
#include <fcopss/cd-asterisk.hpp>

namespace fcopss {

class Cd
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

  Cd();

  explicit
  Cd(const Block& wire);

  Cd(const char* uri);

  Cd(const string& uri);

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
  hasWire() const;

  bool
  equals(const Cd& cd) const;

  bool
  isPrefixOf(const Cd& cd) const;

  bool
  empty() const;

  size_t
  size() const;

  const Block::element_container&
  elements() const;

  const Block&
  get(ssize_t i) const;

  const Block&
  operator[](ssize_t i) const;

  const Block&
  at(ssize_t i) const;

  int
  compare(const Cd& other) const;

  int
  compare(size_t pos1, size_t count1, const Cd& other, size_t pos2 = 0, size_t count2 = npos) const;

  bool
  operator==(const Cd& other) const;

  bool
  operator!=(const Cd& other) const;

  bool
  operator<=(const Cd& other) const;

  bool
  operator<(const Cd& other) const;

  bool
  operator>=(const Cd& other) const;

  bool
  operator>(const Cd& other) const;

private:
  enum State { NotOpened, Opened, Closed };

  enum Event { Hold, Open, Close };

  static void
  parseCdString(vector<string>& words, Block& block);

  static Event
  getEventFromString(string& word);

  static int
  compare(const Block& b1, const Block& b2);

public:
  static const size_t npos;

private:
  mutable Block m_cdBlock;
};

ostream&
operator<<(ostream& os, const Cd& cd);

istream&
operator>>(istream& is, Cd& cd);

} // namespace fcopss

#endif // _FCOPSS_CD_HPP_
