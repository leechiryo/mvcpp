#pragma once

#include <vector>
#include "../Request.h"
#include "../ModelBase.h"
#include "ImgInfoModel.h"

class PicsListModel : public ModelBase
{
  public:
    PicsListModel(const char* foldername, const std::vector<string>& files) 
      : ModelBase("picslist")
    {
      for(auto f : files){
        Add("PICS_LIST", new ImgInfoModel(foldername, f.c_str()));
      }
    }
};

