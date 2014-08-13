#include "AngularTestController.h"
#include "../ControllerFactoryBase.h"

class AngularTestControllerComplete :
  public AngularTestController,
  public ControllerFactory<AngularTestControllerComplete>
{
  public:
  AngularTestControllerComplete():ControllerFactory(m_ctrlName)
  {
    responseTbl["test"] = static_cast<PtrResponseFunction>(&AngularTestController::test);
  }
};

AngularTestControllerComplete angularTestController;
