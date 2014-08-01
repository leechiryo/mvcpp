#ifndef __CONTROLLER_BASE_H__
#define __CONTROLLER_BASE_H__

#include <map>
#include <string>
#include <functional>
#include <iostream>
#include <memory>
#include <vector>
#include <dlfcn.h>

#include "Request.h"
#include "ViewBase.h"
#include "ModelBase.h"
#include "models/DebugInfo.h"

using namespace std;

mutex mtx;
mutex mtx2;
class Controller{

  typedef function< void (const Request &) > ResponseFunction;
  typedef unique_ptr<ModelBase> PtrModel;

  private:
    static map<string, Controller*> s_ctrltbl;
    static vector<void *> s_libs;
    PtrModel m_pmodel;
    bool m_debugMode;

  protected:
    map<string, ResponseFunction> responseTbl;

    Controller(string ctrlname)
    {
      m_debugMode = false;
      auto pos = s_ctrltbl.find(ctrlname);
      if(pos == s_ctrltbl.end())
      {
        // Be careful for the concurency.
        // Can not use the same mutex with the others.
        lock_guard<mutex> lock(mtx2);

        // Search the ctrl name again in the multi thread safe environment.
        auto pos = s_ctrltbl.find(ctrlname);
        if(pos == s_ctrltbl.end())
        {
          // register myself to a global table.
          s_ctrltbl.insert({ctrlname, this});
        }
      }
    }

    void Show(ModelBase *pm)
    {
      m_pmodel = move(PtrModel(pm));
    }

    void SetDebugMode(bool mode)
    {
      m_debugMode = mode;
    }

    void InvokeResponse(const string &path, Request &request)
    {
      request.m_os << "Content-type: text/html\r\n\r\n";
      auto posFunc = responseTbl.find(path);
      if(posFunc != responseTbl.end())
      {
        ResponseFunction responseFunc = posFunc->second;
        responseFunc(request);
        if(m_pmodel) request.m_os << *m_pmodel << endl;
      }
      else
      {
        request.m_os << "Can't find the response for the path: " << path << endl;
      }

      if(m_debugMode)
      {
        DebugInfo m(request);
        request.m_os << m << endl;
      }
    }
    
  public:

    static bool LoadController(string ctrlname)
    {
      auto pos = s_ctrltbl.find(ctrlname);
      if(pos == s_ctrltbl.end())
      {
        // not found in the controller table.
        // load from file.
        
        // Be careful for the concurency.
        // Can not use the same mutex with the others.
        lock_guard<mutex> lock(mtx);

        // Search the ctrl name again in the multi thread safe environment.
        auto pos = s_ctrltbl.find(ctrlname);
        if(pos == s_ctrltbl.end())
        {

          void* dlib = dlopen(("modules/" + ctrlname + ".so").c_str(), RTLD_NOW);
          if(dlib == NULL) 
          {
            string errstr = dlerror();
            return false; // load .so file failed. return false.
          }

          s_libs.push_back(dlib);
        }
      }
      return true;
    }

    static void InvokeResponse(Request &request, const string& ctrlname, const string& action)
    {
      unique_ptr<Controller> pc(s_ctrltbl[ctrlname]->CreateNew());
      pc->InvokeResponse(action, request);
    }

    virtual Controller* CreateNew() = 0;
};

template <class T>
class ControllerBase : public Controller
{
  public:

    ControllerBase(string ctrlname) : Controller(ctrlname) { }

    virtual Controller * CreateNew()
    {
      return new T;
    }
};

#endif
