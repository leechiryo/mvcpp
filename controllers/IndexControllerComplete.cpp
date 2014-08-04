#include "IndexController.h"
#include "../ControllerFactoryBase.h"

class IndexControllerComplete : 
  public IndexController, public ControllerFactory<IndexControllerComplete>
{
  public:
  IndexControllerComplete():ControllerFactory(m_ctrlName)
  {
    responseTbl["welcome"] = static_cast<PtrResponseFunction>(&IndexController::welcome);
    responseTbl["hello"] = static_cast<PtrResponseFunction>(&IndexController::hello);
  }
};

IndexControllerComplete indexController;
