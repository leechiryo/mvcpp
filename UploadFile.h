#pragma once

#include <stdio.h>
#include <string>
using namespace std;

class UploadFile{
private:
  string m_fileName;
  FILE * m_tmpfile;

public:
  UploadFile(string fileName, FILE * tmpfile){
    m_fileName = fileName;
    m_tmpfile = tmpfile;
    if(tmpfile){
      fseek(tmpfile, 0, SEEK_SET);
    }
  }

  virtual ~UploadFile(){
    if(m_tmpfile) {
      fclose(m_tmpfile);
      m_tmpfile = nullptr;
    }
  }

  const string& getName(){
    return m_fileName;
  }

  FILE* getFile() {
    return m_tmpfile;
  }
};
