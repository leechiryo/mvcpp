#include "BuggyController.h"
#include "../ControllerFactoryBase.h"

class BuggyControllerComplete : 
  public BuggyController, 
  public ControllerFactory<BuggyControllerComplete>
{
  public:
  BuggyControllerComplete():ControllerFactory(m_ctrlName)
  {
    responseTbl["fire_segv"] = static_cast<PtrResponseFunction>(&BuggyController::fire_segv);
    responseTbl["fire_fpe"] = static_cast<PtrResponseFunction>(&BuggyController::fire_fpe);
  }
};

BuggyControllerComplete buggyController;
