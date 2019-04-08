/*
  subscribe.cpp

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

class Subscriber
{
public:
  Subscriber(const string cdString, const string filepath)
    : m_cdString(cdString)
    , m_filepath(filepath)
    , m_isFirstRun(true)
  {
  }

  void
  run()
  {
    Cd cd(m_cdString);
    Sub sub(cd);
    
    try
    {
      m_face.subscribe(sub, bind(&Subscriber::onPubRecv, this, _1, _2));
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
  string m_cdString;
  string m_filepath;
  bool   m_isFirstRun;

  void
  onPubRecv(const PubFromRp& pubFromRp, const boost::system::error_code& error)
  {
    if (error)
    {
      if (error == boost::system::errc::operation_canceled)
      {
        return;
      }
      else
      {
        std::cout << error.message() << std::endl;
        m_face.shutdown();
      }
    }
    else
    {
      const Block& wire = pubFromRp.getContent();
      size_t length = wire.value_size();
      const uint8_t* value = wire.value();

      std::ofstream outputFile;
      if (m_isFirstRun)
      {
        outputFile.open(m_filepath);
      }
      else
      {
        outputFile.open(m_filepath, std::ios::app);
      }

      if (outputFile.is_open())
      {
        for (size_t i = 0; i < length; i++)
        {
          outputFile << static_cast<char>(value[i]) << std::flush;
        }

        outputFile.close();
      }
      else
      {
        std::cout << "Failed to open " << m_filepath << " for writing.\n" << std::endl;
      }

      m_isFirstRun = false;
    }
  }

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
    fprintf(stderr, "Usage: %s [-c] CD [-o] FILEPATH\n", argv[0]);
    return 1;
  }

  while ((c = getopt(argc, argv, "c:o:")) != -1)
  {
    switch (c)
    {
    case 'c':
      cd = optarg; 
      break;
    case 'o':
      filepath = optarg; 
      break;
    default:
      fprintf(stderr, "Usage: %s [-c] CD [-o] FILEPATH\n", argv[0]);
      return 2;
    }
  }

  if (cd == nullptr || filepath == nullptr)
  {
    fprintf(stderr, "Usage: %s [-c] CD [-o] FILEPATH\n", argv[0]);
    return 1;
  }
  
  fcopss::client::Subscriber subscriber(cd, filepath);
  subscriber.run();

  return 0;
}
