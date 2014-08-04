#include "ControllerBase.h"
#include "ControllerFactoryBase.h"

void ControllerBase::InvokeResponse(const string &path, Request &request)
{
  request.GetOutput() << "Content-type: text/html\r\n\r\n";
  auto posFunc = responseTbl.find(path);
  if(posFunc != responseTbl.end())
  {
    PtrResponseFunction responseFunc = posFunc->second;
    (this->*responseFunc)(request);
    if(m_pmodel) request.GetOutput() << *m_pmodel << endl;
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