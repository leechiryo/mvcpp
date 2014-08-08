#pragma once

#include "../ModelBase.h"

class ViewInfoModel : public ModelBase
{
  public:
    ViewInfoModel(const char* filename, const char* viewname = "viewinfo")
      : ModelBase(viewname)
    {
      SetShowData("FILE_NAME", filename);
    }
};
