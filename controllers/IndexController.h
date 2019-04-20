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
    string fileName = upfile.getName();
    char str[256] = {0,};
    FILE* tmp = upfile.getFile();
    if (tmp) {
      rewind(tmp);
      char* s = fgets(str, 256, tmp);
      if(!s){
        sprintf (str, "file read error. %s", strerror(errno));
      }
    }
    pm->SetShowData("hellomsg", "hello " + request.GetParam("tbxName") + " " + fileName + "/" + str);
    Show(pm);
  }
};
