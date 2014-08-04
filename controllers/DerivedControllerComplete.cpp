#include "DerivedController.h"
#include "../ControllerFactoryBase.h"

class DerivedControllerComplete : 
  public DerivedController, 
  public ControllerFactory<DerivedControllerComplete>
{
  public:
  DerivedControllerComplete():ControllerFactory(m_ctrlName)
  {
    responseTbl["info"] = static_cast<PtrResponseFunction>(&DerivedController::info);
    responseTbl["inforaw"] = static_cast<PtrResponseFunction>(&DerivedController::inforaw);
    responseTbl["derived"] = static_cast<PtrResponseFunction>(&DerivedController::derived);
  }
};

DerivedControllerComplete derivedController;
