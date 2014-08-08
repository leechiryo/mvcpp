#pragma once

#include "../ModelBase.h"

class ModuleInfoModel : public ModelBase
{
  public:
    ModuleInfoModel(const char* modulename, const char* filename, bool isrunning, 
        const char* viewname = "moduleinfo") : ModelBase(viewname)
    {
      SetShowData("MODULE_NAME", modulename);
      SetShowData("FILE_NAME", filename);
      SetShowData("IS_RUNNING", isrunning?"TRUE":"FALSE");
    }

};
