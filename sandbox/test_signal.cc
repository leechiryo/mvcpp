#include <iostream>
#include <stdlib.h>
#include <unistd.h>

#include "SignalHandler.h"

using namespace std;

struct Some
{
  int a = 0;
  void func()
  {
    cout << a;
  }
};

int main()
{
  int iret;

  try
  {
    SignalHandler s;

    s.setupSignalHandlers();

    Some * so = nullptr;
    so->func();

    iret = EXIT_SUCCESS;
  }
  catch(SignalException& e)
  {
    cerr << "Singal Exception: " << e.what() << endl;
    iret = EXIT_FAILURE;
  }

  return iret;
}
