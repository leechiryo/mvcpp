#pragma once

#include "../ControllerBase.h"
#include "../models/AngularTestModel.h"

class AngularTestController : public ControllerBase
{
  public:
  AngularTestController() : ControllerBase("angular") { }

  void test(const Request &request)
  {
    if(request.GetUrlParams(0) != "")
    {
      Show(new AngularTestModel(request.GetUrlParams(0).c_str()));
    }
    else
    {
      Show(new AngularTestModel());
    }
  }
};

