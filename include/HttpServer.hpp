#ifndef HTTP_SERVER
#define HTTP_SERVER
#include <rte_ring.h>

#include <unordered_map>

class HttpServer {
 public:
  void run();
  HttpServer(rte_ring* rxRing, rte_ring* txRing, rte_ring* freeRing, uint16_t txBurstSize): mRxRing{rxRing}, mTxRing{txRing},
    mFreeRing{freeRing}, mTxBurstSize{txBurstSize} {}
 private:
  rte_ring* const mRxRing;
  rte_ring* const mTxRing;
  rte_ring* const mFreeRing;
  const uint16_t mTxBurstSize;
//  std::unordered_map<std::string, 

};

#endif
