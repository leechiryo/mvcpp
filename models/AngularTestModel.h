#pragma once

#include "../Request.h"
#include "../ModelBase.h"

class AngularTestModel : public ModelBase
{
  public:
    AngularTestModel(const char* viewname = "angular_test") 
      : ModelBase(viewname)
    {
    }
};

