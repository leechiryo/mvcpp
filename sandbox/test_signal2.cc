#include <iostream>
#include <csignal>
#include <stdexcept>
#include <errno.h>
#include <unistd.h>

using namespace std;

class SignalException : public runtime_error
{
  public:
    SignalException(const std::string& _message) : runtime_error(_message)
    {
    }
};

void signalHandler( int signum )
{
  cout << "Interrupt signal (" << signum << ") received.\n";
  exit(1);
}

class Some
{
  int i=0;
  public:
  void seti()
  {
    i=1;
  }
};

int main ()
{
  try
  {
    // register signal SIGINT and signal handler  
    signal(SIGSEGV, signalHandler);  

    while(1){
      cout << "Going to sleep...." << endl;

      Some * so;
      so->seti();
    }

    return 0;
  }
  catch(...)
  {
    cout<< "exception catched" << endl;
    return 1;
  }
}
