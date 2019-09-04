#include "../include/DummyEndpoint.hpp"

void DummyEndpointGet::processRequest(const HttpRequest* request, HttpResponse * response) {
      response->setResponseType(ResponseType::OK);
      response->setResponseVersion(request->getRequestVersion());
      response->setPayload("some small payload");
}
