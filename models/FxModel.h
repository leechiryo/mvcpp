#ifndef __FXMODEL_H__
#define __FXMODEL_H__

#include "../ModelBase.h"

class FxModel : public ModelBase
{
  public:
    FxModel(const char* viewname = "fxctrl") : ModelBase(viewname)
    {
    }
};


#endif
