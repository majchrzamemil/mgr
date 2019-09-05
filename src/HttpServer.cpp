#include "../include/HttpServer.hpp"
#include "../include/HttpRequest.hpp"
#include "../include/HttpResponse.hpp"

void HttpServer::run() {
  HttpRequest* requests[mTxBurstSize];
  Packet* freePackets[mTxBurstSize];
  HttpResponse* responses[mTxBurstSize];

  while (true) {
    uint16_t packetsToFree{0u};
    uint16_t responsesCount{0u};

    auto nrOfRecResp = rte_ring_dequeue_burst(mRxRing, reinterpret_cast<void**>(&requests),
                       mTxBurstSize, nullptr);
    if (nrOfRecResp == 0) {
      continue;
    }
    HttpRequest* request = nullptr;
    for (auto it{0u}; it < nrOfRecResp; ++it) {
      request = requests[it];
      if (unlikely(request->getRequestType() == RequestType::NOT_REQUEST)) {
        freePackets[packetsToFree++] = request->getPacket();
        delete request;
        continue;
      }
      auto response = new HttpResponse(requests[it]->getPacket());
      auto endpoints = mEndpoints[request->getRequestType()];
      if (auto endpoint = endpoints.find(requests[it]->getUri()); endpoint != endpoints.end()) {
        endpoint->second->processRequest(requests[it], response);
      } else {
        responses[it]->setResponseType(ResponseType::NOT_FOUND);
        responses[it]->setResponseVersion(requests[it]->getRequestVersion());
      }
      responses[responsesCount++] =  response;
      delete request;
    }
    rte_ring_enqueue_burst(mFreeRing, reinterpret_cast<void**>(freePackets), packetsToFree, nullptr);
    rte_ring_enqueue_burst(mTxRing, reinterpret_cast<void**>(responses), responsesCount, nullptr);
  }
}

void HttpServer::registerHttpEndpoint(HttpEndpoint* endpoint) {
  mEndpoints[endpoint->getRequestType()][endpoint->getUri()] = endpoint;
}
