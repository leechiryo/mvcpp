#pragma once

#include <map>
#include <string>
#include <time.h>

using namespace std;
class Session {
  private:
    map<string, string> m_sessionInfo;
    time_t m_validUntil;
    const time_t VALID_TIME=1200; // session will be valid in 20 minutes after the last access.

  public:
    string get(string name){
      time_t now = time(0);
      m_validUntil = now + VALID_TIME;
      return m_sessionInfo[name];
    }

    void set(string name, string value){
      time_t now = time(0);
      m_validUntil = now + VALID_TIME;
      m_sessionInfo[name] = value;
    }

    bool hasName(string name){
      time_t now = time(0);
      m_validUntil = now + VALID_TIME;
      return m_sessionInfo.find(name) != m_sessionInfo.end();
    }

    Session(){
      time_t now = time(0);
      m_validUntil = now + VALID_TIME;
    }

    bool isValid(){
      time_t now = time(0);
      return m_validUntil > now;
    }
};
