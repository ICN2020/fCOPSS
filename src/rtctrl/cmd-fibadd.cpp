/*
  cmd-fibadd.cpp

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
#include "cmd-fibadd.hpp"

namespace fcopss {
namespace rtctrl {

CmdFibAdd::CmdFibAdd(int argc, char* argv[], uint16_t ctrlPort, uint16_t routerPort)
  : Cmd(argc, argv, ctrlPort), m_routerPort(routerPort)
{
}

void
CmdFibAdd::parse(int argc, char* argv[])
{
  if (argc == 4) {
    if (strcmp(argv[2], "-f")) {
      BOOST_THROW_EXCEPTION(Error("illegal cmd arg"));
    }
    parseFromFile(argv[3]);
  } else if (argc == 10) {
    parseFromArg(argc, argv);
  } else {
    BOOST_THROW_EXCEPTION(Error("illegal cmd arg"));
  }
}

void
CmdFibAdd::parseFromFile(const string& path)
{
  Request first;
  first.m_cmd = "FIB-DEL";
  m_requests.push_back(first);
  
  try {
    boost::property_tree::ptree tree;
    boost::property_tree::read_ini(path, tree);

    for (uint16_t x = 0; x <= std::numeric_limits<uint16_t>::max(); x++) {
      string pathName = string("FIB.Name") + std::to_string(x);
      boost::optional<string> name = tree.get_optional<string>(pathName);
      if (!name) {
        if (x == 0) {
          BOOST_THROW_EXCEPTION(Error("FIB file error (Name nothing)"));
        }
        break;
      }

      vector<string> faces;
      getFaces(faces, tree, x);

      vector<string> costs;
      getCosts(costs, tree, x);

      if (faces.size() != costs.size()) {
        BOOST_THROW_EXCEPTION(Error("FIB file error (NextHop and Cost count differ"));
      }

      for (size_t i = 0 ; i < faces.size(); i++) {
        string remote;
        string type;
        findTypeRemote(faces[i], tree, type, remote);

        Request request;
        request.m_cmd = "FIB-ADD";
        setParamName(request, *name);
        setParamType(request, type);
        setParamRemote(request, remote, m_routerPort);
        setParamCost(request, costs[i]);

        m_requests.push_back(request);
      }
    }
  } catch (const boost::property_tree::ini_parser_error& e) {
    string msg = e.message() + " (line=" + std::to_string(e.line()) + ")";
    BOOST_THROW_EXCEPTION(Error(msg));
  } catch (const boost::property_tree::ptree_bad_path& e) {
    BOOST_THROW_EXCEPTION(Error(e.what()));
  } catch (const boost::property_tree::ptree_bad_data& e) {
    BOOST_THROW_EXCEPTION(Error(e.what()));
  } catch (const Error& error) {
    BOOST_THROW_EXCEPTION(error);
  } catch (...) {
    BOOST_THROW_EXCEPTION(Error("FIB file error"));
  }
}

void
CmdFibAdd::getFaces(vector<string>& faces, boost::property_tree::ptree& tree, uint16_t x)
{
  string pathName = string("FIB.NextHop") + std::to_string(x);
  string csv = tree.get<string>(pathName);
  splitCsv(faces, csv);
}

void
CmdFibAdd::getCosts(vector<string>& costs, boost::property_tree::ptree& tree, uint16_t x)
{
  string pathName = string("FIB.Cost") + std::to_string(x);
  string csv = tree.get<string>(pathName);
  splitCsv(costs, csv);
}

void
CmdFibAdd::splitCsv(vector<string>& params, string& csv)
{
  std::remove_if(csv.begin(), csv.end(), [](const char& c){return isspace(c);});
  if (csv.empty()) {
    BOOST_THROW_EXCEPTION(Error("FIB file error"));
  }

  boost::split(params, csv, boost::is_any_of(","));
  if (params.empty()) {
    BOOST_THROW_EXCEPTION(Error("FIB file error"));
  }
  for (auto it = params.begin(); it != params.end(); it++) {
    if (it->empty()) {
      BOOST_THROW_EXCEPTION(Error("FIB file error"));
    }
  }
}

void
CmdFibAdd::findTypeRemote(const string& face, boost::property_tree::ptree& tree, string& type, string& remote)
{
  for (uint16_t x = 0; x <= std::numeric_limits<uint16_t>::max(); x++) {
    try {
      string pathName = string("FACES.FaceId") + std::to_string(x);
      string id = tree.get<string>(pathName);
      boost::trim(id);
      if (face == id) {
        pathName = string("FACES.Protocol") + std::to_string(x);
        type = tree.get<string>(pathName);
        pathName = string("FACES.Remote") + std::to_string(x);
        remote = tree.get<string>(pathName);
        return;
      }
    } catch (const boost::property_tree::ptree_bad_path& e) {
      break;
    }
  }
  string msg = "No match [FACES] section data for [FIB] FaceID " + face;
  BOOST_THROW_EXCEPTION(Error(msg));
}

void
CmdFibAdd::parseFromArg(int argc, char* argv[])
{
  Request request;
  request.m_cmd = "FIB-ADD";
  
  for (int i = 2; i < argc; i++) {
    if (!strcmp(argv[i], "-n")) {
      if (++i >= argc) {
        BOOST_THROW_EXCEPTION(Error("illegal cmd arg: name missing ?"));
      }
      setParamName(request, argv[i]);
      continue;
    }
    if (!strcmp(argv[i], "-t")) {
      if (++i >= argc) {
        BOOST_THROW_EXCEPTION(Error("illegal cmd arg: type missing ?"));
      }
      setParamType(request, argv[i]);
      continue;
    }
    if (!strcmp(argv[i], "-r")) {
      if (++i >= argc) {
        BOOST_THROW_EXCEPTION(Error("illegal cmd arg: remote missing ?"));
      }
      setParamRemote(request, argv[i], m_routerPort);
      continue;
    }
    if (!strcmp(argv[i], "-c")) {
      if (++i >= argc) {
        BOOST_THROW_EXCEPTION(Error("illegal cmd arg: cost missing ?"));
      }
      setParamCost(request, argv[i]); 
      continue;
    }
    BOOST_THROW_EXCEPTION(Error("illegal cmd arg: unkonwn option"));
  }
  if (request.m_param.size() != 5) {
    BOOST_THROW_EXCEPTION(Error("illegal cmd arg: missing/duplicating option"));
  }

  m_requests.push_back(request);
}

void
CmdFibAdd::setParamCost(Request& request, const string& cost)
{
  try {
    boost::lexical_cast<uint32_t>(cost);
  } catch (...) {
    BOOST_THROW_EXCEPTION(Error("illegal cmd arg: " + cost));
  }

  request.m_param["COST"] = cost;
}

} // namespace rtctrl
} // namespace fcopss
