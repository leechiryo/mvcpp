#pragma once

#include "../ControllerBase.h"

class BuggyController : public ControllerBase
{
  public:
  BuggyController() : ControllerBase("buggy") { }

  /*
   * segment violation will be fired for test
   */
  void fire_segv(const Request &request)
  {
    string *str = nullptr;
    str->push_back('a');
  }

  /*
   * divide by zero will be fired for test
   */
  void fire_fpe(const Request &request)
  {
    int a = 100;
    int b = 0;
    int c = a / b;
  }
};
