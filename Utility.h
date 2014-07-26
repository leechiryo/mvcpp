#ifndef __UTILITY_H__
#define __UTILITY_H__

#include <string>
#include <sstream>

using namespace std;

class Utility
{
  public:

  /*
   * Decode the http request string using utf8.
   *
   */
  static string HttpStrDecode(const string& data)
  {
    ostringstream os;
    string::size_type pos = 0;

    while(pos != data.length())
    {
      auto idx = data.find('%', pos);
      if(idx != string::npos)
      {
        // found!!
        os << data.substr(pos, idx-pos);
        // push the following 2 char as char code into the string stream.
        os << static_cast<char>(stoi(data.substr(idx+1, 2), nullptr, 16));
        pos = idx + 3;
      }
      else
      {
        // no % in the last string.
        os << data.substr(pos);
        pos = data.length();
      }
    }

    os << flush;

    return os.str();
  }

  /*
   * in the request uri like "/fcgi/soname/action/..."
   * find the segment following the /fcgi/
   * it will be used as share object name and the action name.
   */
  static void GetUrlTagName(string requestUri, string& soname, string& action)
  {
    auto startPos = requestUri.find("/fcgi/", 0);

    soname="";
    action="";

    if(startPos != string::npos)
    {
      startPos += 6;
      auto endPos = requestUri.find('/', startPos);
      if(endPos != string::npos)
      {
        soname = requestUri.substr(startPos, endPos-startPos);
        auto actStart = endPos + 1;
        auto actEnd = requestUri.find('/', actStart);
        if(actEnd != string::npos)
          action = requestUri.substr(actStart, actEnd-actStart);
        else
          action = requestUri.substr(actStart);
      }
      else
      {
        soname = requestUri.substr(startPos);
      }

    }
  }

};

#endif
