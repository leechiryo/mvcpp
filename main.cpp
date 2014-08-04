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

typedef unique_ptr<FCGX_Request> PtrFCGXRequest;

void ResponseRequest(PtrFCGXRequest request)
{
  Request req(*request);

  // get .so library file name, and action name.
  string soname;
  string action;
  Utility::GetUrlTagName(req.documentUri, soname, action);

  // load the library.
  if(ControllerFactoryBase::LoadController(soname))
  {
    ControllerBase::InvokeResponse(req, soname, action);
  }
  else
  {
    // failed to load a library.
    req.GetOutput() << "Content-type: text/html\r\n\r\n";
    req.GetOutput() << "Can't initialize the controller: " << soname << endl;
  }

  FCGX_Finish_r(request.get());
}

int main() {

  FCGX_Init();

  while(true){

    PtrFCGXRequest request(new FCGX_Request);
    FCGX_InitRequest(request.get(), 0, 0);

    if (FCGX_Accept_r(request.get()) == 0)
    {
      //ResponseRequest(move(request));
      thread response_thread(ResponseRequest, move(request));
      response_thread.detach();
    }

  }
  return 0;
}


