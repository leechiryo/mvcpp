#include "../ControllerBase.h"
#include "../models/Index.h"
#include "../models/DebugInfo.h"

class IndexController : public ControllerBase<IndexController>
{
  public:
  IndexController() : ControllerBase("index")
  {
    SetDebugMode(true);

    responseTbl["welcome"] = static_cast<PtrResponseFunction>(&IndexController::welcome);
    responseTbl["hello"] = static_cast<PtrResponseFunction>(&IndexController::hello);
  }

  void welcome(const Request &request)
  {
    ModelBase * pm = new Index();
    Show(pm);
  }

  void hello(const Request &request)
  {
    ModelBase * pm = new Index();
    pm->SetShowData("hellomsg", "hello " + request.GetParam("tbxName"));
    Show(pm);
  }
};

IndexController me;
