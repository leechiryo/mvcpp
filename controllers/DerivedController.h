#pragma once

#include "TestController.h"

class DerivedController : public TestController
{
  public:
  DerivedController() : TestController()
  {
    m_ctrlName = "derived";
  }

  void derived(const Request &request)
  {
    ModelBase * pm = new DebugInfo(request, nullptr);
    Show(pm);
  }
};

