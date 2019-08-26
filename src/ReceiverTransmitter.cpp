#include "../include/ReceiverTransmitter.hpp"
#include "../include/DpdkEngine.hpp"

#include <rte_ether.h>
#include <rte_mbuf.h>

#include <iostream>

ReceiverTransmitter::ReceiverTransmitter(rte_ring* const rxRing, rte_ring* const txRing, rte_ring* const freeRing,
    Engine* const engine):  mEngine{engine}, mRxRing{rxRing}, mTxRing{txRing}, mFreeRing{freeRing}  {}

void ReceiverTransmitter::run() {
//maybe smarter
  while (true) {
    receivePackets();
    sendPackets();
  //  mEngine->freePackets(mFreeRing);

  }
}

void ReceiverTransmitter::receivePackets() {
  Packet*  packets[RX_BURST_SIZE];
  auto nrOfRecPkts = mEngine->receivePackets(packets);
  if (nrOfRecPkts == 0u) {
    return;
  }
  for (auto it{0u}; it < nrOfRecPkts; ++it) {
    ether_hdr* eth  = reinterpret_cast<ether_hdr*>(packets[it]->getData());
    std::cout << unsigned(eth->s_addr.addr_bytes[5]) << std::endl;
 //   eth = rte_pktmbuf_mtod(packets[it]->getMBuf(), ether_hdr*);
    ether_addr d_addr = eth->d_addr;
    eth->d_addr = eth->s_addr;
    eth->s_addr = d_addr;
    
  }
  //test:
  mEngine->sendPackets(packets, nrOfRecPkts);

  //only for test print and free
  for (auto pktId{0u}; pktId < nrOfRecPkts; ++pktId) {
    //just free test
   // rte_ring_sp_enqueue(mFreeRing, rxPackets[pktId]);
  }

}

void ReceiverTransmitter::sendPackets() {
// rte_ring_sc_dequeue_bulk(mTxRing, )
}
