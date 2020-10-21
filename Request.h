#pragma once

#include <iostream>
#include <map>
#include <vector>
#include <sstream>

#include <fcgio.h>

#include "RequestStream.h"
#include "UploadFile.h"
#include "Session.h"

using namespace std;

class ControllerBase;

class Request
{
  friend class ControllerBase;

private:
  fcgi_streambuf m_cout_fcgi_streambuf;
  ostream m_os;
  map<string, string> m_params;
  vector<string> m_urlPartials;
  string nullstr;
  string m_boundary;
  UploadFile m_emptyFile;
  map<string, UploadFile> m_files;

  void SplitParams(const string data)
  {
    string::size_type pos = 0;

    while (pos != data.length())
    {
      auto idx = data.find('&', pos);
      if (idx != string::npos)
      {
        // found the &!!
        // get the substring before &;
        AddParam(data.substr(pos, idx - pos));
        pos = idx + 1;
      }
      else
      {
        // no & in the string.
        // all the string will be parsed as one parameter
        AddParam(data.substr(pos));
        break;
      }
    }
  }

  void AddParam(const string & str)
  {
    auto idx = str.find('=');
    if (idx != string::npos)
    {
      // found the =!!
      // get the substring before = as param name and 
      // after = is the part of value.
      string key(move(HttpStrDecode(str.substr(0, idx))));
      if (m_params.find(key) == m_params.end())
      {
        m_params.insert(make_pair(key, HttpStrDecode(str.substr(idx + 1))));
      }
    }
  }

  /*
   * Decode the http request string using utf8.
   */
  string HttpStrDecode(const string & data)
  {
    ostringstream os;
    string::size_type pos = 0;

    while (pos != data.length())
    {
      auto idx = data.find('%', pos);
      if (idx != string::npos)
      {
        // found!!
        os << data.substr(pos, idx - pos);
        // push the following 2 char as char code into the string stream.
        os << static_cast<char>(stoi(data.substr(idx + 1, 2), nullptr, 16));
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
  void GetUrlPartials(const string & requestUri)
  {
    m_urlPartials.clear();

    auto startPos = requestUri.find("/fcgi/", 0);
    if (startPos != string::npos)
    {
      startPos += 6;
      auto endPos = requestUri.find('/', startPos);
      while (endPos != string::npos)
      {
        m_urlPartials.push_back(requestUri.substr(startPos, endPos - startPos));
        startPos = endPos + 1;
        endPos = requestUri.find('/', startPos);
      }

      if (startPos != string::npos)
      {
        m_urlPartials.push_back(requestUri.substr(startPos, endPos - startPos));
      }
    }
  }

  void ReadBoundary(const string & contentType) {
    auto pos = contentType.find("boundary=", 0);
    if (pos != string::npos) {
      pos += 9;
      m_boundary = "--";
      m_boundary += contentType.substr(pos);
    }
  }

  int searchInBuf(const char* buf, int startIdx, int endIdx, const char* key) {
    auto keyLen = strlen(key);
    int searchIdx = 0;
    for (int i = startIdx; i <= endIdx; i++) {
      if (buf[i] == key[searchIdx]) {
        searchIdx++;
        if (searchIdx >= keyLen) {
          return i;
        }
      }
      else {
        searchIdx = 0;
      }
    }

    return -1;
  }

  string Parse(const char* multiContents, int startIdx, int endIdx, const char* key) {
    int keyLen = strlen(key);
    if (keyLen > 97) return nullstr;

    char skey[100];
    strcpy(skey, key);
    strcpy(skey + keyLen, "=\"");
    keyLen += 2;
    skey[keyLen] = 0;

    int idx = searchInBuf(multiContents, startIdx, endIdx, skey);
    if (idx == -1) return nullstr;

    while (multiContents[idx - keyLen] != ' ' && multiContents[idx - keyLen] != ';') {
      idx = searchInBuf(multiContents, idx + 1, endIdx, skey);
      if (idx == -1) return nullstr;
    }

    int idx2 = searchInBuf(multiContents, idx + 1, endIdx, "\"");
    if (idx2 == -1) return nullstr;

    string retval(multiContents + idx + 1, multiContents + idx2);
    return retval;
  }

  /*
   * Parse param from multi-part form data.
   */
  void ParseParams(istream & fin, int contLen) {
    RequestStream rs{ &fin,  &m_boundary, static_cast<size_t>(contLen) };
    rs.SeekToNextSection();
    string header;

    while (rs.ReadHeader(header)) {

      string name = Parse(header.c_str(), 0, header.size() - 1, "name");
      string filename = Parse(header.c_str(), 0, header.size() - 1, "filename");
      if (name != nullstr) {
        string key(move(HttpStrDecode(name)));
        if (filename == nullstr) {
          // common input fields.
          string val;
          rs.ReadTextBody(val);
          if (m_params.find(key) == m_params.end()) {
            m_params.insert(make_pair(key, HttpStrDecode(val)));
          }
        }
        else {
          // uploaded file.
          if (FILE * file = tmpfile()) {
            char buf[2048];
            size_t size = 0;
            while(!rs.ReadFileBody(buf, 2048, size)){
              fwrite(buf, 1, size, file);
            }
            fwrite(buf, 1, size, file);
            if (m_files.find(key) == m_files.end()) {
              UploadFile up(filename, file);
              m_files.insert(make_pair(key, move(up)));
            }
          }
        }
      }

    }
  }

public:
  string queryString, requestMethod, contentType, contentLength,
    scriptName, requestUri, documentUri, documentRoot,
    serverProtocol, gatewayInterface, serverSoftware,
    remoteAddr, remotePort, serverAddr, serverPort,
    serverName, postContents, httpCookie;

  Request(const FCGX_Request & request)
    : m_cout_fcgi_streambuf(request.out),
    m_os(&m_cout_fcgi_streambuf),
    nullstr(""), m_boundary(""), m_emptyFile("", nullptr)
  {
    // get raw params from environment viables
    queryString = FCGX_GetParam("QUERY_STRING", request.envp);
    requestMethod = FCGX_GetParam("REQUEST_METHOD", request.envp);
    contentType = FCGX_GetParam("CONTENT_TYPE", request.envp);
    contentLength = FCGX_GetParam("CONTENT_LENGTH", request.envp);
    scriptName = FCGX_GetParam("SCRIPT_NAME", request.envp);
    requestUri = FCGX_GetParam("REQUEST_URI", request.envp);
    documentUri = FCGX_GetParam("DOCUMENT_URI", request.envp);
    documentRoot = FCGX_GetParam("DOCUMENT_ROOT", request.envp);
    serverProtocol = FCGX_GetParam("SERVER_PROTOCOL", request.envp);
    gatewayInterface = FCGX_GetParam("GATEWAY_INTERFACE", request.envp);
    serverSoftware = FCGX_GetParam("SERVER_SOFTWARE", request.envp);
    remoteAddr = FCGX_GetParam("REMOTE_ADDR", request.envp);
    remotePort = FCGX_GetParam("REMOTE_PORT", request.envp);
    serverAddr = FCGX_GetParam("SERVER_ADDR", request.envp);
    serverPort = FCGX_GetParam("SERVER_PORT", request.envp);
    serverName = FCGX_GetParam("SERVER_NAME", request.envp);
    httpCookie = "<NULL>";
    if(FCGX_GetParam("HTTP_COOKIE", request.envp) != nullptr){
      httpCookie = FCGX_GetParam("HTTP_COOKIE", request.envp);
    }

    // analyse the query string (must be in the encoded status).
    m_params.clear();
    m_files.clear();
    SplitParams(queryString);

    // read the post contents.
    postContents = "";
    if (contentLength.length() > 0)
    {
      int contLen = stoi(contentLength);
      fcgi_streambuf cin_fcgi_streambuf(request.in);
      istream fin(&cin_fcgi_streambuf);

      // analyse the post contents(must be in the encoded status).
      if (contentType == "application/x-www-form-urlencoded") {
        char* buf = new char[contLen + 1];
        fin.read(buf, contLen);
        buf[contLen] = 0;
        postContents = HttpStrDecode(buf);
        SplitParams(buf);
        delete[] buf;
      }
      else if (contentType.compare(0, 20, "multipart/form-data;") == 0) {
        ReadBoundary(contentType);
        if (m_boundary != "") {
          ParseParams(fin, contLen);
        }
      }
    }

    // get url partials.
    GetUrlPartials(documentUri);
  }

  const string& GetParam(const string & paramName) const
  {
    try { return m_params.at(paramName); }
    catch (...) { return nullstr; }
  }

  const UploadFile* GetFile(const string & paramName) const
  {
    try { return &(m_files.at(paramName)); }
    catch (...) { return &m_emptyFile; }
  }

  ostream& GetOutput()
  {
    return m_os;
  }

  const string& GetSoName() const
  {
    if (m_urlPartials.size() > 0)
    {
      return m_urlPartials[0];
    }

    return nullstr;
  }

  const string& GetAction() const
  {
    if (m_urlPartials.size() > 1)
    {
      return m_urlPartials[1];
    }

    return nullstr;
  }

  const string& GetUrlParams(int idx) const
  {
    if (m_urlPartials.size() > idx + 2)
    {
      return m_urlPartials[idx + 2];
    }

    return nullstr;
  }
};
