#ifndef HTTP_SERVER
#define HTTP_SERVER
#include <rte_ring.h>

#include <unordered_map>

#include "HttpEndpoint.hpp"

class HttpServer {
 public:
  HttpServer(rte_ring* rxRing, rte_ring* txRing, rte_ring* freeRing, uint16_t txBurstSize): mRxRing{rxRing},
    mTxRing{txRing},
    mFreeRing{freeRing}, mTxBurstSize{txBurstSize} {}
  void run();
  void registerHttpEndpoint(HttpEndpoint* endpoint);
 private:
  rte_ring* const mRxRing;
  rte_ring* const mTxRing;
  rte_ring* const mFreeRing;
  const uint16_t mTxBurstSize;
  constexpr static uint8_t mRequestCount{4u};
  std::unordered_map<std::string, HttpEndpoint*> mEndpoints[mRequestCount];

};

#endif
