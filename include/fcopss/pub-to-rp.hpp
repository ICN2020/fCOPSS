/*
  pub-to-rp.hpp

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
#ifndef _FCOPSS_PUBTORP_HPP_
#define _FCOPSS_PUBTORP_HPP_

#include <fcopss/common.hpp>
#include <fcopss/pub.hpp>

namespace fcopss {

class PubToRp : public Pub
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

  PubToRp();

  PubToRp(const Cd& cd);

  PubToRp(const PubToRp&) = default;

  PubToRp(PubToRp&&) = default;

  explicit
  PubToRp(const Block& wire);

  virtual
  ~PubToRp();

  PubToRp&
  operator=(const PubToRp&) = default;
  PubToRp&
  operator=(PubToRp&&) = default;

  template<ndn::encoding::Tag TAG>
  size_t
  wireEncode(ndn::encoding::EncodingImpl<TAG>& encoder) const;

  const Block&
  wireEncode() const override;

  void
  wireDecode(const Block& wire) override;

  PubToRp&
  setCd(const Cd& cd);

  PubToRp&
  setContent(const uint8_t* buffer, size_t bufferSize);

  PubToRp&
  setContent(const Block& block);

  bool
  operator==(const PubToRp& other) const;

  bool
  operator!=(const PubToRp& other) const;
};

ostream&
operator<<(ostream& os, const PubToRp& pub);

} // namespace fcopss

#endif // _FCOPSS_PUBTORP_HPP_
