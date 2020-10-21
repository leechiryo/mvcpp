#include "ControllerBase.h"
#include "ControllerFactoryBase.h"

void ControllerBase::InvokeResponse(const string &path, Request &request)
{
  // output the response header.
  request.GetOutput() << "Content-type: ";
  request.GetOutput() << m_responseMime;
  request.GetOutput() << "\r\n";

  auto pos = request.httpCookie.find("MVCPP_SESSION=");
  if(pos != string::npos) {
    // Get the session id
    string idStr;
    auto pos2 = request.httpCookie.find(";", pos);
    if(pos2==string::npos) {
      idStr = request.httpCookie.substr(pos + 14);
    } else {
      idStr = request.httpCookie.substr(pos + 14, pos2 - pos - 14);
    }

    // Try to get the session object from session id 
    if(s_sessions.find(idStr) != s_sessions.end()){
      mySession = s_sessions[idStr];
    }
  }
  
  if(mySession == nullptr) {

    // If the session does not found, create a new one.
    //
    // create a new session id first.
    uuid_t uuid;
    uint32_t status;
    uuid_create(&uuid, &status);
    char * id;
    uuid_to_string(&uuid, &id, &status);
    string idStr(id);
    free(id);
    id = nullptr;

    // set the new session id to cookie.
    request.GetOutput() << "Set-Cookie: MVCPP_SESSION=" << idStr << "; SameSite=Lax\r\n";

    // create a new Session object with the session id.
    Session* newSession = new Session();
    mySession = newSession;
    s_sessions.insert(make_pair(idStr, newSession));

  }

  request.GetOutput() << "\r\n";

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
