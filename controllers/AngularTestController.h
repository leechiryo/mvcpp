#pragma once

#include "../ControllerBase.h"
#include "../models/AngularTestModel.h"

class AngularTestController : public ControllerBase
{
  public:
  AngularTestController() : ControllerBase("angular") { }

  void test(const Request &request)
  {
    Show(new AngularTestModel());
  }
};

