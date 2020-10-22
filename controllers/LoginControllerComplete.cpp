#include "LoginController.h"
#include "../ControllerFactoryBase.h"

class LoginControllerComplete :
  public LoginController,
  public ControllerFactory<LoginControllerComplete>
{
  public:
  LoginControllerComplete():ControllerFactory(m_ctrlName)
  {
    responseTbl["act"] = static_cast<PtrResponseFunction>(&LoginController::act);
  }
};

LoginControllerComplete loginController;
