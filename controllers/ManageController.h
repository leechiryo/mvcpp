#pragma once

#include "../ControllerBase.h"
#include "../models/ManageModel.h"
#include "../ControllerFactoryBase.h"

class ManageController : public ControllerBase
{
  public:
  ManageController() : ControllerBase("manage") { }

  void list(const Request &request)
  {
    Show(new ManageModel(ControllerFactoryBase::s_ctrltbl));
  }

  void listraw(const Request &request)
  {
    Show(new ManageModel(ControllerFactoryBase::s_ctrltbl, nullptr));
  }
};
