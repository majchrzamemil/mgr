#include "../include/ReceiverTransmitter.hpp"
#include "../include/DpdkEngine.hpp"

#include <rte_mbuf.h>

#include <iostream>

ReceiverTransmitter::ReceiverTransmitter(rte_ring* rxRing, rte_ring* txRing, EngineType type, int dpdkArgc,
    char** dpdkArgv):  mRxRing{rxRing}, mTxRing{txRing} {
  if (type == EngineType::DPDK) {
    mEngine = std::make_unique<DpdkEngine>();
    //add some flag if engine is running or throw exc
    if (mEngine->init(dpdkArgc, dpdkArgv)) {
      mEngine->startEngine();
    }
  }
}

void ReceiverTransmitter::run() {
//maybe smarter
  while (true) {
    receivePackets();
  }
}

void ReceiverTransmitter::receivePackets() {
  ether_hdr* rxPackets[RX_BURST_SIZE];
  //rte_mbuf* mbufs[RX_BURST_SIZE];
  auto nrOfRecPkts = mEngine->receivePackets(rxPackets);
  if(nrOfRecPkts == 0u)
    return;
  //only for test print and free
  std::cout << "numer of packets:" << unsigned(nrOfRecPkts) << "\n";
  for (auto pktId{0u}; pktId < nrOfRecPkts; ++pktId) {

    // rte_pktmbuf_free(rxPackets[pktId]);
  }
}

void ReceiverTransmitter::sendPackets() {

}
