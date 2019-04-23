#ifndef __UploadFile_H__
#define __UploadFile_H__

#include "../ModelBase.h"

class UploadModel : public ModelBase
{
  public:
    UploadModel(const char* viewname = "upload") : ModelBase(viewname)
    {
    }
};


#endif
