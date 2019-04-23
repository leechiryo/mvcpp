#pragma once

#include <istream>
#include <string>
#include <deque>

using namespace std;
class RequestStream {

private:
  istream* m_stream;
  const string* m_boundary;
  string m_boundary2;
  size_t m_length;
  size_t m_readPtr;
  deque<char> escbuf;

public:
  RequestStream(istream* stream, const string* boundary, size_t length) {
    m_stream = stream;
    m_boundary = boundary;
    m_boundary2 = "\r\n" + *m_boundary;
    m_readPtr = 0;
    m_length = length;
  }

private:
  // found: return 1
  // not found until the end of the stream: return 0
  // and the read pointer is located at the end of the stream.
  int SeekUntil(const string& key) {
    auto keyLen = key.size();
    int searchIdx = 0;
    char c;
    while (m_readPtr++ < m_length) {
      m_stream->get(c);
      if (c == key[searchIdx]) {
        searchIdx++;
        if (searchIdx >= keyLen) {
          return 1;
        }
      }
      else {
        searchIdx = 0;
      }
    }

    return 0;
  }

  bool ReadUntil(char* buf, size_t bufSize, const string& key, size_t& cpySize) {
    auto keyLen = key.size();
    cpySize = 0;
    char c;
    static int lastChar = -1;

    if (lastChar != -1) {
      while (escbuf.size()) {
        buf[cpySize++] = escbuf.front();
        escbuf.pop_front();
        if (cpySize >= bufSize) {
          return false;
        }
      }

      buf[cpySize++] = static_cast<char>(lastChar);
      lastChar = -1;
      if (cpySize >= bufSize) {
        return false;
      }
    }

    while (m_readPtr++ < m_length) {
      m_stream->get(c);
      if (c == key[escbuf.size()]) {
        escbuf.push_back(c);
        if (escbuf.size() >= keyLen) {
          escbuf.clear();
          return true;
        }
      }
      else {
        while (escbuf.size()) {
          buf[cpySize++] = escbuf.front();
          escbuf.pop_front();
          if (cpySize >= bufSize) {
            lastChar = static_cast<unsigned char>(c);
            return false;
          }
        }
        buf[cpySize++] = c;
        if (cpySize >= bufSize) {
          return false;
        }
      }
    }

    return false;
  }

public:

  int SeekToNextSection() {
    return SeekUntil(*m_boundary);
  }

  int ReadHeader(string& header) {
    char buf[2048];
    size_t size;
    ReadUntil(buf, 2048, "\r\n\r\n", size);
    if (size >= 2048) {
      throw runtime_error("error: header is too large.");
    }

    buf[size] = 0;
    header = buf;
    return size;
  }

  size_t ReadTextBody(string& body) {
    char buf[2048];
    size_t readBytes = 0;
    size_t size = 0;

    while (!ReadUntil(buf, 2048, m_boundary2, size)) {
      body.append(buf, size);
      readBytes += size;
    }
    body.append(buf, size);
    readBytes += size;

    return readBytes;
  }

  bool ReadFileBody(char* buf, size_t bufSize, size_t& readSize) {
    return ReadUntil(buf, bufSize, m_boundary2, readSize);
  }
};
