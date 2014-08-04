#include "TestController.h"
#include "../ControllerFactoryBase.h"

class TestControllerComplete : 
  public TestController, 
  public ControllerFactory<TestControllerComplete>
{
  public:
  TestControllerComplete():ControllerFactory(m_ctrlName)
  {
    responseTbl["info"] = static_cast<PtrResponseFunction>(&TestController::info);
    responseTbl["inforaw"] = static_cast<PtrResponseFunction>(&TestController::inforaw);
  }
};

TestControllerComplete testController;
