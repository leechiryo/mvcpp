#pragma once

#include <map>
#include <string>

using namespace std;
class Session {
  private:
    map<string, string> m_sessionInfo;

  public:
    string get(string name){
      return m_sessionInfo[name];
    }

    void set(string name, string value){
      m_sessionInfo[name] = value;
    }

    bool hasName(string name){
      return m_sessionInfo.find(name) != m_sessionInfo.end();
    }
};
