#pragma once

#include <dirent.h>
#include <cstring>

#include "../ModelBase.h"
#include "../ControllerFactoryBase.h"
#include "ModuleInfoModel.h"
#include "ViewInfoModel.h"

class ManageModel : public ModelBase
{
  typedef map<string, ControllerFactoryBase*> CtrlTbl;
  public:
    ManageModel(const CtrlTbl& ctrltbl, const char* viewname = "manage") 
      : ModelBase(viewname)
    {
      DIR *dpdf;
      struct dirent *epdf;

      // get the modules info.
      dpdf = opendir("modules");
      if (dpdf != NULL)
      {
        while ((epdf = readdir(dpdf)))
        {
          char *filename = epdf->d_name;
          if(strcmp(filename+strlen(filename)-3, ".so") == 0)
          {
            char modulename[256] = {0,};
            strcpy(modulename, filename);
            modulename[strlen(filename)-3] = 0;

            auto pos = ctrltbl.find(modulename);
            if(pos == ctrltbl.end())
            {
              Add("MODULES_INFO", new ModuleInfoModel(modulename, filename, false));
            }
            else
            {
              Add("MODULES_INFO", new ModuleInfoModel(modulename, filename, true));
            }
          }
        }
      }
      closedir(dpdf);

      // get the views info.
      dpdf = opendir("views");
      if (dpdf != NULL)
      {
        while ((epdf = readdir(dpdf)))
        {
          char *filename = epdf->d_name;
          if(strcmp(filename+strlen(filename)-5, ".html") == 0
             || strcmp(filename+strlen(filename)-4, ".htm") == 0)
          {
            Add("VIEWS_INFO", new ViewInfoModel(filename));
          }
        }
      }
      closedir(dpdf);
    }
};
