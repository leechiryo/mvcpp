#pragma once

#include "../ModelBase.h"

class ImgInfoModel : public ModelBase
{
  public:
    ImgInfoModel(const char* foldername, const char* filename) : ModelBase("imginfo")
    {
      SetShowData("FOLDER_NAME", foldername);
      SetShowData("FILE_NAME", filename);
    }
};
