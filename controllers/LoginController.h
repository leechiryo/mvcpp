#include "../ControllerBase.h"

class LoginController : public ControllerBase
{
  public:
  LoginController() : ControllerBase("login")
  { }

  void act(const Request &request)
  {
    // Do the security check.
    string name = request.GetParam("tbxName");
    string pass = request.GetParam("tbxPass");
    if(name != "" && pass == "liz123"){
      mySession->set("name", name);
      RedirectTo("/fcgi/index/welcome");
    } else {
      RedirectTo("/login.html");
    }
  }
};
