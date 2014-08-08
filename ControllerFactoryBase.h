#ifndef __CONTROLLER_FACTORY_BASE_H__
#define __CONTROLLER_FACTORY_BASE_H__

#include "ControllerBase.h"

class ManageController;

class ControllerFactoryBase
{
  friend class ControllerBase;
  friend class ManageController;

  static map<string, ControllerFactoryBase*> s_ctrltbl;
  static vector<void *> s_libs;

  public:
    virtual ControllerBase* CreateNew() = 0;
    ControllerFactoryBase(const string& ctrlname);
    static bool LoadController(const string& ctrlname);
};

template <class T>
class ControllerFactory : public ControllerFactoryBase
{
  public:
    ControllerFactory(const string& ctrlname) : ControllerFactoryBase(ctrlname){ } 

    virtual ControllerBase * CreateNew()
    {
      return new T;
    }
};

#endif
