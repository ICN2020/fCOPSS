/*
  pub-from-rp.hpp

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
#ifndef _FCOPSS_PUBFROMRP_HPP_
#define _FCOPSS_PUBFROMRP_HPP_

#include <fcopss/common.hpp>
#include <fcopss/pub.hpp>

namespace fcopss {

class PubToRp;

class PubFromRp : public Pub
{
public:

  class Error : public Pub::Error
  {
  public:
    explicit
    Error(const string& what)
      : Pub::Error(what) 
    {
    }
  };

  PubFromRp();

  PubFromRp(const Cd& cd);

  PubFromRp(const PubToRp& pub);

  PubFromRp(const PubFromRp&) = default;

  PubFromRp(PubFromRp&&) = default;

  explicit
  PubFromRp(const Block& wire);

  virtual
  ~PubFromRp();

  PubFromRp&
  operator=(const PubFromRp&) = default;
  PubFromRp&
  operator=(PubFromRp&&) = default;

  template<ndn::encoding::Tag TAG>
  size_t
  wireEncode(ndn::encoding::EncodingImpl<TAG>& encoder) const;

  virtual const Block&
  wireEncode() const override;

  virtual void
  wireDecode(const Block& wire) override;

  PubFromRp&
  setCd(const Cd& cd);

  PubFromRp&
  setContent(const uint8_t* buffer, size_t bufferSize);

  PubFromRp&
  setContent(const Block& block);

  bool
  operator==(const PubFromRp& other) const;

  bool
  operator!=(const PubFromRp& other) const;
};

ostream&
operator<<(ostream& os, const PubFromRp& pub);

} // namespace fcopss

#endif // _FCOPSS_PUBFROMRP_HPP_
