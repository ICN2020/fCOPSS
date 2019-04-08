/*
  sub.hpp

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
#ifndef _FCOPSS_SUB_HPP_
#define _FCOPSS_SUB_HPP_

#include <fcopss/common.hpp>
#include <fcopss/cd.hpp>

namespace fcopss {

class Sub
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

  Sub();

  Sub(const Cd& cd);

  explicit
  Sub(const Block& wire);

  template<ndn::encoding::Tag TAG>
  size_t
  wireEncode(ndn::encoding::EncodingImpl<TAG>& encoder) const;

  const Block&
  wireEncode() const;

  void
  wireDecode(const Block& wire);

  bool
  hasWire() const;

  const Cd&
  getCd() const;

  Sub&
  setCd(const Cd& cd);

  bool
  operator==(const Sub& other) const;

  bool
  operator!=(const Sub& other) const;

private:
  Cd m_cd;

  mutable Block m_wire;
};

ostream&
operator<<(ostream& os, const Sub& sub);

} // namespace fcopss

#endif // _FCOPSS_SUB_HPP_
