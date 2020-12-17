#include "Utility.h"
#include "ControllerBase.h"
#include "ControllerFactoryBase.h"

void ControllerBase::OutputResponse(Request& request) {
  ostream& httpOut = request.GetOutput();
  // header
  httpOut << "Status: ";
  httpOut << m_status;
  httpOut << "\r\n";
  httpOut << "Content-type: ";
  httpOut << m_responseMime;
  httpOut << "\r\n";
  if(m_status == 302) {
    httpOut << "Location: ";
    httpOut << m_location;
    httpOut << "\r\n";
  }
  if(m_cookie != "") {
    httpOut << "Set-Cookie: ";
    httpOut << m_cookie;
    httpOut << " Path=/; SameSite=Lax\r\n";
  }
  httpOut << "\r\n";

  // body
  if(m_status == 200 && m_pmodel) {
    httpOut << *m_pmodel << endl;
  }
  else {
    httpOut << m_error << endl;
  }

  if(m_debugMode)
  {
    DebugInfo m(request);
    httpOut << m << endl;
  }
}

void ControllerBase::InvokeResponse(const string &path, Request &request)
{
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
    m_cookie = format("MVCPP_SESSION=%s;", idStr);

    // Create a new Session object for the session id.
    Session* newSession = new Session();
    mySession = newSession;
    s_sessions[idStr] = newSession;

    free(idStr);
    idStr = nullptr;
  }

  // Check the session if the name is not exist, redirect to login page.
  // The action 'login/act' will save the name to session.
  if(!mySession->hasName("name") 
     && (m_ctrlName != "login" || path != "act")){
    m_status = 302;
    m_location = "/login.html";
    return;
  }

  // Process the request if it is a login user.
  auto posFunc = responseTbl.find(path);
  if(posFunc != responseTbl.end())
  {
    PtrResponseFunction responseFunc = posFunc->second;

    // Set jump for the system error in the response function.
    int syserrno = 0;
    if((syserrno = sigsetjmp(s_jbuf, 1)) == 0)
    {
      (this->*responseFunc)(request);
    }
    else
    {
      // System error (SIGSEGV, SIGFPE etc.) occured in the response function.
      // Output the error message and return.
      m_status = 500;
      m_error = format("System error occured. Errno: %d Path: %s\r\n", syserrno, path.c_str());
    }
  }
  else
  {
    m_status = 404;
    m_error = format("Can't find the response for the path: %s\r\n", path.c_str());
  }
}

void ControllerBase::InvokeResponse(Request &request,
                                    const string& ctrlname,
                                    const string& action)
{
  unique_ptr<ControllerBase> pc(ControllerFactoryBase::s_ctrltbl[ctrlname]->CreateNew());
  pc->InvokeResponse(action, request);
  pc->OutputResponse(request);
}
