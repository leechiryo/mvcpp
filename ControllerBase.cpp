#include "ControllerBase.h"
#include "ControllerFactoryBase.h"

void ControllerBase::InvokeResponse(const string &path, Request &request)
{
  // output the response header.
  request.GetOutput() << "Content-type: ";
  request.GetOutput() << m_responseMime;
  request.GetOutput() << "\r\n\r\n";

  auto posFunc = responseTbl.find(path);
  if(posFunc != responseTbl.end())
  {
    PtrResponseFunction responseFunc = posFunc->second;

    // Set jump for the system error in the response function.
    int syserrno = 0;
    if((syserrno = sigsetjmp(s_jbuf, 1)) == 0)
    {
      (this->*responseFunc)(request);
      if(m_pmodel) request.GetOutput() << *m_pmodel << endl;
    }
    else
    {
      // system error (SIGSEGV, SIGFPE etc.) occured in the response function.
      // output the error message and return.
      request.GetOutput() << "System error occured. Errno: " << syserrno 
                          << " Path: " << path << endl;
    }
  }
  else
  {
    request.GetOutput() << "Can't find the response for the path: " << path << endl;
  }

  if(m_debugMode)
  {
    DebugInfo m(request);
    request.GetOutput() << m << endl;
  }
}

void ControllerBase::InvokeResponse(Request &request,
                                    const string& ctrlname,
                                    const string& action)
{
  unique_ptr<ControllerBase> pc(ControllerFactoryBase::s_ctrltbl[ctrlname]->CreateNew());
  pc->InvokeResponse(action, request);
}
