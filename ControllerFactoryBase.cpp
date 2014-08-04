#include "ControllerFactoryBase.h"

mutex mtx;
mutex mtx2;

ControllerFactoryBase::ControllerFactoryBase(const string& ctrlname)
{
  auto pos = s_ctrltbl.find(ctrlname);
  if(pos == s_ctrltbl.end())
  {
    // Be careful for the concurency.
    // Do not use the same mutex with the others.
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

bool ControllerFactoryBase::LoadController(const string& ctrlname)
{
  auto pos = s_ctrltbl.find(ctrlname);
  if(pos == s_ctrltbl.end())
  {
    // not found in the controller table.
    // load from file.
    
    // Be careful for the concurency.
    // Do not use the same mutex with the others.
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
