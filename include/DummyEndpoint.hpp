#ifndef DUMMY_ENDPOINT
#define DUMMY_ENDPOINT
#include "HttpEndpoint.hpp"

class DummyEndpointGet : public HttpEndpoint {
 public:
  DummyEndpointGet(std::string uri, RequestType requestType) : HttpEndpoint(uri, requestType) {}
  void processRequest(const HttpRequest* request, HttpResponse* response) override;
  ~DummyEndpointGet() {};
};

#endif
