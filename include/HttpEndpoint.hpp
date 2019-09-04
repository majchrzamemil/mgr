#ifndef HTTP_ENDPOINT
#define HTTP_ENDPOINT
#include <string>

#include "HttpRequest.hpp"
#include "HttpResponse.hpp"

class HttpEndpoint {
 public:
  HttpEndpoint(std::string uri, RequestType requestType): mUri{uri}, mRequestType{requestType} {}
  virtual void processRequest(const HttpRequest* request, HttpResponse* response) = 0;
  std::string getUri() const {
    return mUri;
  }
  RequestType getRequestType() const {
    return mRequestType;
  }
  virtual ~HttpEndpoint() {};
 protected:
  std::string mUri;
  RequestType mRequestType;
};

#endif
