#include "../ControllerBase.h"
#include "../models/DebugInfo.h"

class TestController : public ControllerBase<TestController>
{
  public:
  TestController() : ControllerBase("debug")
  {
    responseTbl["info"] = static_cast<PtrResponseFunction>(&TestController::info);
    responseTbl["inforaw"] = static_cast<PtrResponseFunction>(&TestController::inforaw);
  }

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

TestController me;
