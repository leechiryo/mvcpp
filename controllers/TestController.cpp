#include "../ControllerBase.h"
#include "../models/DebugInfo.h"

class TestController : public ControllerBase
{
  public:
  TestController() : ControllerBase("debug")
  {
    responseTbl["info"] = [&](const Request &request) {
      Show(new DebugInfo(request));
    };

    responseTbl["inforaw"] = [&](const Request &request) {
      ModelBase * pm = new DebugInfo(request, nullptr);
      Show(pm);
    };

  }
};

TestController me;
