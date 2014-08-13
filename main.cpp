#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <memory>
#include <thread>

#include "/usr/local/include/fcgio.h"

#include "Request.h"
#include "ControllerBase.h"
#include "ControllerFactoryBase.h"

using namespace std;

map<string, ControllerFactoryBase*> ControllerFactoryBase::s_ctrltbl;
vector<void *> ControllerFactoryBase::s_libs;
thread_local sigjmp_buf ControllerBase::s_jbuf;

typedef unique_ptr<FCGX_Request> PtrFCGXRequest;

void ResponseRequest(PtrFCGXRequest request)
{
  Request req(*request);

  // load the library.
  if(ControllerFactoryBase::LoadController(req.GetSoName()))
  {
    ControllerBase::InvokeResponse(req, req.GetSoName(), req.GetAction());
  }
  else
  {
    // failed to load a library.
    req.GetOutput() << "Content-type: text/html\r\n\r\n";
    req.GetOutput() << "Can't initialize the controller: " << req.GetSoName() << endl;
  }

  FCGX_Finish_r(request.get());
}

/*
 * Signal handler for system error in the response function.
 */
static void response_sys_err_handler (int sig, siginfo_t *siginfo, void *context)
{
  siglongjmp(ControllerBase::s_jbuf, sig);
}

int main() {

  FCGX_Init();

  /* Set the signal handler for the system error
   * SIGBUS, SIGFPE, SIGILL and SIGSEGV
   * that may occured in response function. */
  struct sigaction act;
  memset (&act, '\0', sizeof(act));

  /* Use the sa_sigaction field because the handles has two additional parameters */
  act.sa_sigaction = &response_sys_err_handler;

  /* The SA_SIGINFO flag tells sigaction() to use the sa_sigaction field, not sa_handler. */
  act.sa_flags = SA_SIGINFO;

  if (sigaction(SIGBUS, &act, NULL) < 0) {
    printf("error sigaction");
    return 1;
  }
  if (sigaction(SIGFPE, &act, NULL) < 0) {
    printf("error sigaction");
    return 1;
  }
  if (sigaction(SIGILL, &act, NULL) < 0) {
    printf("error sigaction");
    return 1;
  }
  if (sigaction(SIGSEGV, &act, NULL) < 0) {
    printf("error sigaction");
    return 1;
  }

  while(true){

    PtrFCGXRequest request(new FCGX_Request);
    FCGX_InitRequest(request.get(), 0, 0);

    if (FCGX_Accept_r(request.get()) == 0)
    {
      thread response_thread(ResponseRequest, move(request));
      response_thread.detach();
    }

  }
  return 0;
}


