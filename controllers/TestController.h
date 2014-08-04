#ifndef __TEST_CONTROLLER_H__
#define __TEST_CONTROLLER_H__

#include "../ControllerBase.h"
#include "../models/DebugInfo.h"

class TestController : public ControllerBase
{
  public:
  TestController() : ControllerBase("debug") { }

  void info(const Request &request)
  {
    Show(new DebugInfo(request));
  }

  void inforaw(const Request &request)
  {
    ModelBase * pm = new DebugInfo(request, nullptr);
    Show(pm);
  }
};

#endif
