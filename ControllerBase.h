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

  protected:
    // pointer to response function
    typedef void (ControllerBase::* PtrResponseFunction)(const Request&);
    // response function table
    map<string, PtrResponseFunction> responseTbl;
    PtrModel m_pmodel;
    bool m_debugMode;
    string m_ctrlName;

    ControllerBase(const string& ctrlname)
    {
      m_debugMode = false;
      m_ctrlName = ctrlname;
    }

    void Show(ModelBase *pm)
    {
      m_pmodel = move(PtrModel(pm));
    }

    void SetDebugMode(bool mode)
    {
      m_debugMode = mode;
    }
    
  public:
    static void InvokeResponse(Request &request, const string& ctrlname, const string& action);
    static thread_local sigjmp_buf s_jbuf;
};

#endif
