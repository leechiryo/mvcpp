#include <stdio.h>
#include "../ControllerBase.h"
#include "../models/FxModel.h"
#include "../models/DebugInfo.h"

class FxController : public ControllerBase
{
  public:
  FxController() : ControllerBase("fxctrl")
  {
  }

  void set(const Request &request)
  {
    string po = request.GetParam("policy");
    string postr;
    ModelBase * pm = new FxModel();
    if(po != ""){
      FILE *fp = NULL;
      fp = fopen("static/fxsgnl.js", "w");
      if(fp){
        fputs(po.c_str(), fp);
        fclose(fp);
      }
      else{
        postr = "error: write file open error.";
        pm->SetShowData("policy", postr);
        Show(pm);
        return;
      }
    }

    FILE *fpr = NULL;
    fpr = fopen("static/fxsgnl.js", "r");
    int pocode = 0;
    if(fpr){
      pocode = fgetc(fpr);
      fclose(fpr);
      pm->SetShowData("policy", std::to_string(pocode-48));
      Show(pm);
      return;
    }
    else{
      postr = "error: read file open error.";
      pm->SetShowData("policy", postr);
      Show(pm);
      return;
    }
  }
};
