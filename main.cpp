#include <iostream>
#include <sstream>
#include <fstream>
#include <string>

#include "/usr/local/include/fcgio.h"

#include "Request.h"
#include "ControllerBase.h"

using namespace std;

map<string, ControllerBase*> ControllerBase::s_ctrltbl;
vector<void *> ControllerBase::s_libs;

int main() {

  FCGX_Request request;

  FCGX_Init();
  FCGX_InitRequest(&request, 0, 0);

  while(FCGX_Accept_r(&request) == 0) {
    Request req(request);

    // get .so library file name, and action name.
    string soname;
    string action;
    Utility::GetUrlTagName(req.documentUri, soname, action);

    // load the library.
    if(ControllerBase::LoadController(soname))
    {
      ControllerBase::InvokeResponse(req, soname, action);
    }
  }

  return 0;
}


