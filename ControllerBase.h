#ifndef __CONTROLLER_BASE_H__
#define __CONTROLLER_BASE_H__

#include <map>
#include <string>
#include <iostream>
#include <memory>
#include <vector>
#include <dlfcn.h>
#include <signal.h>
#include <thread>
#include <setjmp.h>
#include <uuid.h>
#include <functional>

#include "Session.h"
#include "Request.h"
#include "ViewBase.h"
#include "ModelBase.h"
#include "models/DebugInfo.h"

using namespace std;

class ControllerBase
{
  private:
    typedef unique_ptr<ModelBase> PtrModel;
    void InvokeResponse(const string &path, Request &request);
    void OutputResponse(Request& request);
    string m_responseMime;
    static map<string, Session*> s_sessions;

    int m_status;
    string m_cookie;
    string m_location;
    string m_error;

  protected:
    // pointer to response function
    typedef void (ControllerBase::* PtrResponseFunction)(const Request&);
    // response function table
    map<string, PtrResponseFunction> responseTbl;
    PtrModel m_pmodel;
    bool m_debugMode;
    string m_ctrlName;
    Session* mySession;

    ControllerBase(const string& ctrlname, const char* responseMime="text/html") 
    {
      m_debugMode = false;
      m_ctrlName = ctrlname;
      m_responseMime = responseMime;
      mySession = nullptr;
      m_status = 200;
    }

    void Show(ModelBase *pm)
    {
      m_pmodel = move(PtrModel(pm));
    }

    void SetDebugMode(bool mode)
    {
      m_debugMode = mode;
    }

    void RedirectTo(const string& path){
      m_status = 302;
      m_location = path;
    }
    
  public:
    static void InvokeResponse(Request &request, const string& ctrlname, const string& action);
    static thread_local sigjmp_buf s_jbuf;
};

#endif
