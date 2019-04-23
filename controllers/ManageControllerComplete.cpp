#include "ManageController.h"
#include "../ControllerFactoryBase.h"

class ManageControllerComplete :
  public ManageController,
  public ControllerFactory<ManageControllerComplete>
{
  public:
  ManageControllerComplete():ControllerFactory(m_ctrlName)
  {
    responseTbl["list"] = static_cast<PtrResponseFunction>(&ManageController::list);
    responseTbl["listraw"] = static_cast<PtrResponseFunction>(&ManageController::listraw);
  }
};

ManageControllerComplete manageController;
