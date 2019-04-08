/*
  publish.cpp

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
#include <unistd.h>
#include <cctype>
#include <cstdio>
#include <iostream>
#include <fcopss/face.hpp>


namespace fcopss {
namespace client {

class Publisher
{
public:
  Publisher(const string cd, const string filepath)
    : m_cd(cd)
    , m_filepath(filepath)
  {
  }

  void
  run()
  {
    Cd cd(m_cd);
    PubToRp pubToRp(cd);

    std::ifstream inputFile(m_filepath, std::ios::binary | std::ios::ate);
    if (inputFile.fail())
    {
      std::cout << "Failed to open " << m_filepath << std::endl;
      return;
    }
    
    std::streampos begin, end;
    end = inputFile.tellg();
    inputFile.seekg(0, std::ios::beg);
    begin = inputFile.tellg();
    int filesize = end - begin;
    std::vector<char> dataVector(filesize);
    inputFile.read(dataVector.data(), filesize);
    inputFile.close();
    
    pubToRp.setContent(reinterpret_cast<uint8_t*>(dataVector.data()), dataVector.size());

    try
    {
      m_face.publish(pubToRp);
      std::cout << "Bytes Sent: " << dataVector.size() << std::endl;
      m_face.run();
    }
    catch (const std::exception& e)
    {
      std::cerr << "ERROR: " << e.what() << std::endl;
    }

    return;
  }

private:
  Face   m_face;
  string m_cd;
  string m_filepath;

};

}   // namespace client
}   // namepsace fcopss

int
main(int argc, char** argv)
{
  char* cd = nullptr;
  char* filepath = nullptr;
  int c;

  if (argc < 5)
  {
    fprintf(stderr, "Usage: %s [-c] CD [-i] FILEPATH\n", argv[0]);
    return 1;
  }

  while ((c = getopt(argc, argv, "c:i:")) != -1)
  {
    switch (c)
    {
    case 'c':
      cd = optarg; 
      break;
    case 'i':
      filepath = optarg; 
      break;
    default:
      fprintf(stderr, "Usage: %s [-c] CD [-i] FILEPATH\n", argv[0]);
      return 2;
    }
  }

  if (cd == nullptr || filepath == nullptr)
  {
    fprintf(stderr, "Usage: %s [-c] CD [-i] FILEPATH\n", argv[0]);
    return 1;
  }

  fcopss::client::Publisher publisher(cd, filepath);
  publisher.run();

  return 0;
}
