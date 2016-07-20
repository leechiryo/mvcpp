#pragma once

#include <dirent.h>
#include <vector>
#include "../ControllerBase.h"
#include "../models/PicsListModel.h"

class PicsController : public ControllerBase
{
  public:
  PicsController() : ControllerBase("pics") { }

  void login(const Request &request){
  }

  void list(const Request &request)
  {
    string folder = request.GetParam("f");
    DIR *d;
    struct dirent *dir;
    d = opendir(("/home/leechiryo/www/static/pics/" + folder).c_str());
    std::vector<string> files;
    if(d){
      while ((dir = readdir(d)) != NULL){
        if(strcmp(dir->d_name + strlen(dir->d_name) - 4, ".JPG") == 0
            && (dir->d_type == DT_REG || dir->d_type == DT_LNK)){
          files.push_back(dir->d_name);
        }
      }
    }
    
    Show(new PicsListModel(folder.c_str(), files));
  }

  void detail(const Request &request) {
  }
};
