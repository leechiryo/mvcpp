#include "../ControllerBase.h"
#include "../models/Index.h"
#include "../models/DebugInfo.h"

class IndexController : public ControllerBase
{
  public:
  IndexController() : ControllerBase("index")
  {
    SetDebugMode(true);

    responseTbl["welcome"] = [&](const Request &request) {
      ModelBase * pm = new Index();
      Show(pm);
    };

    responseTbl["hello"] = [&](const Request &request) {
      ModelBase * pm = new Index();
      pm->SetShowData("hellomsg", "hello " + request.GetParam("tbxName"));
      Show(pm);
    };
  }
};

IndexController me;
