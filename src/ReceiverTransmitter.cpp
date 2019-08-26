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
    mEngine->freePackets(mFreeRing);

  }
}

void ReceiverTransmitter::receivePackets() {
  Packet*  packets[RX_BURST_SIZE];
  auto nrOfRecPkts = mEngine->receivePackets(packets);
  if (nrOfRecPkts == 0u) {
    return;
  }
 // for (auto it{0u}; it < nrOfRecPkts; ++it) {
 //   ether_hdr* eth  = reinterpret_cast<ether_hdr*>(packets[it]->getData());
 //   ether_addr d_addr = eth->d_addr;
 //   eth->d_addr = eth->s_addr;
 //   eth->s_addr = d_addr;
 //   
 // }
  //test:
//  mEngine->sendPackets(packets, nrOfRecPkts);

  auto rt =rte_ring_enqueue_burst(mRxRing, reinterpret_cast<void**>(packets), nrOfRecPkts, nullptr);
  for (auto pktId{rt}; pktId < nrOfRecPkts; ++pktId) {
    rte_ring_sp_enqueue(mFreeRing, rxPackets[pktId]);
  }

}

void ReceiverTransmitter::sendPackets() {
  Packet* txPackets[RX_BURST_SIZE];
  auto nrOfRecPkts = rte_ring_dequeue_burst(mTxRing, reinterpret_cast<void**>(txPackets), RX_BURST_SIZE, nullptr);
  if(nrOfRecPkts == 0)
    return;
  mEngine->sendPackets(txPackets, nrOfRecPkts);
}
