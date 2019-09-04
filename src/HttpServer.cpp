#include "../include/HttpServer.hpp"
#include "../include/HttpRequest.hpp"
#include "../include/HttpResponse.hpp"


void HttpServer::run() {
  HttpRequest* requests[mTxBurstSize];
//  Packet* freePackets[mTxBurstSize];
  HttpResponse* responses[mTxBurstSize];

  while (true) {
 //   uint16_t packetsToFree{0u};
    uint16_t responsesCount{0u};

    auto nrOfRecResp = rte_ring_dequeue_burst(mRxRing, reinterpret_cast<void**>(&requests), mTxBurstSize, nullptr);
    if (nrOfRecResp == 0) {
      continue;
    }
    for(auto it{0u}; it < nrOfRecResp; ++it) {
      //check uri if not preset, free;
      HttpResponse* response = new HttpResponse(requests[it]->getPacket()); 
      response->setResponseType(ResponseType::OK);
      response->setResponseVersion(requests[it]->getRequestVersion());
      responses[responsesCount++] =  response;
      delete requests[it];
    }
//    rte_ring_enqueue_burst(mFreeRing, reinterpret_cast<void**>(freePackets), pktsToFree, nullptr);
    rte_ring_enqueue_burst(mTxRing, reinterpret_cast<void**>(responses), responsesCount, nullptr);
  }
}
