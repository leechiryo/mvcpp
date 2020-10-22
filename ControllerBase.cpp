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
      if(!mySession->isValid()){
        // If the session is timeout, then delete it.
        s_sessions.erase(idStr);
	delete mySession;
	mySession = nullptr;
      }
    }
  }
  
  if(mySession == nullptr) {

    // If the session is not found, create a new one.
   
    // Create a new session id first.
    uuid_t uuid;
    uint32_t status;
    uuid_create(&uuid, &status);
    char * idStr;
    uuid_to_string(&uuid, &idStr, &status);

    // Set the new session id to cookie.
    request.GetOutput() << "Set-Cookie: MVCPP_SESSION=" << idStr << "; SameSite=Lax\r\n";

    // Create a new Session object for the session id.
    Session* newSession = new Session();
    mySession = newSession;
    s_sessions[idStr] = newSession;

    free(idStr);
    idStr = nullptr;
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
      // System error (SIGSEGV, SIGFPE etc.) occured in the response function.
      // Output the error message and return.
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
