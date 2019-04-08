/*
  pub.hpp

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
#ifndef _FCOPSS_PUB_HPP_
#define _FCOPSS_PUB_HPP_

#include <fcopss/common.hpp>
#include <fcopss/cd.hpp>

namespace fcopss {

class Pub
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

  Pub();

  Pub(const Cd& cd);

  Pub(const Pub&) = default;

  Pub(Pub&&) = default;

  virtual
  ~Pub();

  Pub&
  operator=(const Pub&) = default;
  Pub&
  operator=(Pub&&) = default;

  virtual const Block&
  wireEncode() const = 0;

  virtual void
  wireDecode(const Block& wire) = 0;

  bool
  hasWire() const;

  const Cd&
  getCd() const;

  Pub&
  setCd(const Cd& cd);

  const Block&
  getContent() const;

  Pub&
  setContent(const uint8_t* buffer, size_t bufferSize);

  Pub&
  setContent(const Block& block);

  bool
  operator==(const Pub& other) const;

  bool
  operator!=(const Pub& other) const;

protected:
  Cd m_cd;
  mutable Block m_content;

  mutable Block m_wire;
};

ostream&
operator<<(ostream& os, const Pub& pub);

} // namespace fcopss

#endif // _FCOPSS_PUB_HPP_
