#include "../include/ReceiverTransmitter.hpp"
#include "../include/DpdkEngine.hpp"

#include <rte_mbuf.h>

#include <iostream>

ReceiverTransmitter::ReceiverTransmitter(rte_ring* rxRing, rte_ring* txRing,rte_ring* freeRing, EngineType type, int dpdkArgc,
    char** dpdkArgv):  mRxRing{rxRing}, mTxRing{txRing}, mFreeRing{freeRing} {
  if (type == EngineType::DPDK) {
    mEngine = std::make_unique<DpdkEngine>();
    //add some flag if engine is running or throw exc
    if (mEngine->init(dpdkArgc, dpdkArgv)) {
      mEngine->startEngine();
      mFreeRing = rte_ring_create("freeRing2", TX_BURST_SIZE, SOCKET_ID_ANY, 0);
    }
  }
}

void ReceiverTransmitter::run() {
//maybe smarter
  while (true) {
    receivePackets();
    mEngine->freePackets(mFreeRing);
    
  }
}

void ReceiverTransmitter::receivePackets() {
  Packet* rxPackets[RX_BURST_SIZE];
  //rte_mbuf* mbufs[RX_BURST_SIZE];
  auto nrOfRecPkts = mEngine->receivePackets(rxPackets);
  if(nrOfRecPkts == 0u){
    return;
  
  }
  //only for test print and free
  for (auto pktId{0u}; pktId < nrOfRecPkts; ++pktId) {
    //just free test
   rte_ring_sp_enqueue(mFreeRing, rxPackets[pktId]);
  }
  
}

void ReceiverTransmitter::sendPackets() {

}
