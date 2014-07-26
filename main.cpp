#include <iostream>
#include <sstream>
#include <fstream>
#include <string>

#include "/usr/local/include/fcgio.h"

#include "Request.h"
#include "ControllerBase.h"

using namespace std;

map<string, Controller*> Controller::s_ctrltbl;
vector<void *> Controller::s_libs;

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
    if(Controller::LoadController(soname))
    {
      Controller::InvokeResponse(req, soname, action);
    }
    else
    {
      // failed to load a library.
      req.GetOutput() << "Content-type: text/html\r\n\r\n";
      req.GetOutput() << "Can't initialize the controller: " << soname << endl;
    }
  }

  return 0;
}


