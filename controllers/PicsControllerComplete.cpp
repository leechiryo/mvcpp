#include "PicsController.h"
#include "../ControllerFactoryBase.h"

class PicsControllerComplete :
  public PicsController,
  public ControllerFactory<PicsControllerComplete>
{
  public:
  PicsControllerComplete():ControllerFactory(m_ctrlName)
  {
    responseTbl["login"] = static_cast<PtrResponseFunction>(&PicsController::login);
    responseTbl["list"] = static_cast<PtrResponseFunction>(&PicsController::list);
    responseTbl["detail"] = static_cast<PtrResponseFunction>(&PicsController::detail);
  }
};

PicsControllerComplete picsController;
