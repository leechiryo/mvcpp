#include <errno.h>
#include "../ControllerBase.h"
#include "../models/Index.h"
#include "../models/DebugInfo.h"

class IndexController : public ControllerBase
{
  public:
  IndexController() : ControllerBase("index")
  {
    SetDebugMode(true);
  }

  void welcome(const Request &request)
  {
    ModelBase * pm = new Index();
    Show(pm);
  }

  void hello(const Request &request)
  {
    ModelBase * pm = new Index();
    auto upfile  = request.GetFile("file1");
    string fileName = upfile->getName();
    char str[256] = {0,};
    FILE* tmp = upfile->getFile();
    if (tmp) {
      char* s = fgets(str, 256, tmp);
      if(!s){
        sprintf (str, "file read error. %s", strerror(errno));
      }
    }

    string name = request.GetParam("tbxName");
    if(mySession != nullptr){
      if(name != ""){
        mySession->set("name", name);
      } else if (name == "" && mySession->hasName("name")) {
        name = mySession->get("name");
      }
    }

    pm->SetShowData("hellomsg", "hello " + name + " " + fileName + "/" + str);
    Show(pm);
  }
};
