/*
  cmd-fibadd.hpp

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
#ifndef _FCOPSS_RTCTRL_CMD_FIBADD_HPP_
#define _FCOPSS_RTCTRL_CMD_FIBADD_HPP_

#include <fcopss/common.hpp>

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/ini_parser.hpp>
#include <boost/optional.hpp>

#include "cmd.hpp"

namespace fcopss {
namespace rtctrl {

class CmdFibAdd : public Cmd
{
public:
  CmdFibAdd(int argc, char* argv[], uint16_t ctrlPort, uint16_t routerPort);

  void
  parse(int argc, char* argv[]) override;

private:
  void
  parseFromFile(const string& path);

  void
  getFaces(vector<string>& faces, boost::property_tree::ptree& tree, uint16_t x);

  void
  getCosts(vector<string>& costs, boost::property_tree::ptree& tree, uint16_t x);

  void
  splitCsv(vector<string>& params, string& csv);

  void
  findTypeRemote(const string& face, boost::property_tree::ptree& tree, string& type, string& remote);

  void
  parseFromArg(int argc, char* argv[]);

  void
  setParamCost(Request& request, const string& cost);

private:
  uint16_t m_routerPort;
};

} // namespace rtctrl
} // namespace fcopss

#endif // _FCOPSS_RTCTRL_CMD_FIBADD_HPP_


