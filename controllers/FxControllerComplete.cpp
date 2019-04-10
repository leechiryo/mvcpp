#include "FxController.h"
#include "../ControllerFactoryBase.h"

class FxControllerComplete :
  public FxController,
  public ControllerFactory<FxControllerComplete>
{
  public:
  FxControllerComplete():ControllerFactory(m_ctrlName)
  {
    responseTbl["set"] = static_cast<PtrResponseFunction>(&FxController::set);
  }
};

FxControllerComplete fxController;
