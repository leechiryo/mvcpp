#ifndef __DEBUG_INFO_H__
#define __DEBUG_INFO_H__

#include "../Request.h"
#include "../ModelBase.h"

class DebugInfo : public ModelBase
{
  public:
    DebugInfo(const Request &request, const char* viewname = "debug") 
      : ModelBase(viewname)
    {
      m_showData["QUERY_STRING"] = request.queryString;
      m_showData["REQUEST_METHOD"] = request.requestMethod;
      m_showData["CONTENT_TYPE"] = request.contentType;
      m_showData["CONTENT_LENGTH"] = request.contentLength;
      m_showData["SCRIPT_NAME"] = request.scriptName;
      m_showData["REQUEST_URI"] = request.requestUri;
      m_showData["DOCUMENT_URI"] = request.documentUri;
      m_showData["DOCUMENT_ROOT"] = request.documentRoot;
      m_showData["SERVER_PROTOCOL"] = request.serverProtocol;
      m_showData["GATEWAY_INTERFACE"] = request.gatewayInterface;
      m_showData["SERVER_SOFTWARE"] = request.serverSoftware;
      m_showData["REMOTE_ADDR"] = request.remoteAddr;
      m_showData["REMOTE_PORT"] = request.remotePort;
      m_showData["SERVER_ADDR"] = request.serverAddr;
      m_showData["SERVER_PORT"] = request.serverPort;
      m_showData["SERVER_NAME"] = request.serverName;
      m_showData["POST_CONTENTS"] = request.postContents;
      m_showData["HTTP_COOKIE"] = request.httpCookie;
    }
};

#endif
