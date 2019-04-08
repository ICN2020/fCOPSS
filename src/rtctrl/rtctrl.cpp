/*
  rtctrl.cpp

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
#include "rtctrl.hpp"

namespace fcopss {
namespace rtctrl {

Rtctrl::Rtctrl(const Config& config)
{
  m_routerPort = config.routerPort();
  m_ctrlPort = config.routerControlPort();
}

void
Rtctrl::run(int argc, char* argv[])
{
  try {
    Cmd* cmd = createCmd(argc, argv);
    cmd->execute();
  } catch (const Error& error) {
    std::cerr << "error: " << error.what() << std::endl;
    std::cerr << std::endl;
    usage();
  }
}

Cmd*
Rtctrl::createCmd(int argc, char* argv[])
{
  Cmd* cmd = nullptr;

  CmdType cmdType = parseCmdType(argc, argv);
  if (cmdType == FibAdd) {
    cmd = new CmdFibAdd(argc, argv,  m_ctrlPort, m_routerPort);
  } else if (cmdType == FibDel) {
    cmd = new CmdFibDel(argc, argv, m_ctrlPort, m_routerPort);
  } else if (cmdType == StDel) {
    cmd = new CmdStDel(argc, argv, m_ctrlPort);
  } else if (cmdType == FaceDel) {
    cmd = new CmdFaceDel(argc, argv, m_ctrlPort);
  } else if (cmdType == StDump) {
    cmd = new CmdStDump(argc, argv, m_ctrlPort);
  } else if (cmdType == FibDump) {
    cmd = new CmdFibDump(argc, argv, m_ctrlPort);
  } else if (cmdType == FaceDump) {
    cmd = new CmdFaceDump(argc, argv, m_ctrlPort);
  }
 
  return cmd;
}

Rtctrl::CmdType
Rtctrl::parseCmdType(int argc, char* argv[])
{
  if (argc < 2) {
    BOOST_THROW_EXCEPTION(Error("unknown cmd type"));
  }

  CmdType cmdType;

  string typeArg = argv[1];
  if (typeArg == "fibadd") {
    cmdType = FibAdd;
  } else if (typeArg == "fibdel") {
    cmdType = FibDel;
  } else if (typeArg == "fibdump") {
    cmdType = FibDump;
  } else if (typeArg == "stdel") {
    cmdType = StDel;
  } else if (typeArg == "stdump") {
    cmdType = StDump;
  } else if (typeArg == "facedel") {
    cmdType = FaceDel;
  } else if (typeArg == "facedump") {
    cmdType = FaceDump;
  } else {
    BOOST_THROW_EXCEPTION(Error("unkown cmd type"));
  }

  return cmdType;
}

void
Rtctrl::usage() const
{
  std::cerr << "usage:" << std::endl;
  std::cerr << std::endl;

  std::cerr << "  For add a FIB entry..." << std::endl;
  std::cerr << "    rtctrl fibadd -n NAME -t TYPE -r REMOTE[:PORT] -c COST" << std::endl;
  std::cerr << "      NAME: name prefix" << std::endl;
  std::cerr << "      TYPE: tcp or udp" << std::endl;
  std::cerr << "      REMOTE: remote hostname or IPv4 dotted decimal" << std::endl;
  std::cerr << "      PORT: port number" << std::endl;
  std::cerr << "      COST: cost of this route" << std::endl;
  std::cerr << std::endl;

  std::cerr << "  For add FIB entries from file..." << std::endl;
  std::cerr << "    rtctrl fibadd -f FILE" << std::endl;
  std::cerr << "      FILE: path of FIB file" << std::endl;
  std::cerr << std::endl;

  std::cerr << "  For delete a FIB entry..." << std::endl;
  std::cerr << "    rtctrl fibdel -n NAME [-t TYPE -r REMOTE[:PORT]]" << std::endl;
  std::cerr << "      NAME: name prefix" << std::endl;
  std::cerr << "      TYPE: tcp or udp" << std::endl;
  std::cerr << "      REMOTE: remote hostname or IPv4 dotted decimal" << std::endl;
  std::cerr << "      PORT: port number" << std::endl;
  std::cerr << std::endl;

  std::cerr << "  For delete all FIB entry..." << std::endl;
  std::cerr << "    rtctrl fibdel" << std::endl;
  std::cerr << std::endl;

  std::cerr << "  For print all FIB entry..." << std::endl;
  std::cerr << "    rtctrl fibdump" << std::endl;
  std::cerr << std::endl;

  std::cerr << "  For delete all ST entry..." << std::endl;
  std::cerr << "    rtctrl stdel" << std::endl;
  std::cerr << std::endl;

  std::cerr << "  For print all ST entry..." << std::endl;
  std::cerr << "    rtctrl stdump" << std::endl;
  std::cerr << std::endl;

  std::cerr << "  For delete a Face entry..." << std::endl;
  std::cerr << "    rtctrl facedel -i ID" << std::endl;
  std::cerr << "      ID: Face ID" << std::endl;
  std::cerr << std::endl;

  std::cerr << "  For print all Face entry..." << std::endl;
  std::cerr << "    rtctrl facedump" << std::endl;
  std::cerr << std::endl;
}

} // namespace rtctrl
} // namespace fcopss
