#include "../include/ReceiverTransmitter.hpp"
#include "../include/DpdkEngine.hpp"

#include <rte_ether.h>
#include <rte_mbuf.h>

#include <iostream>

ReceiverTransmitter::ReceiverTransmitter(rte_ring* const rxRing, rte_ring* const txRing, rte_ring* const freeRing,
    Engine* const engine):  mEngine{engine}, mRxRing{rxRing}, mTxRing{txRing}, mFreeRing{freeRing}  {}

void ReceiverTransmitter::run() {
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

  auto rt = rte_ring_enqueue_burst(mRxRing, reinterpret_cast<void**>(packets), nrOfRecPkts, nullptr);
  if (rt != nrOfRecPkts) {
    rte_ring_enqueue_burst(mFreeRing, reinterpret_cast<void**>(packets + rt), nrOfRecPkts - rt, nullptr);
  }
}

void ReceiverTransmitter::sendPackets() {
  Packet* txPackets[RX_BURST_SIZE];
  auto nrOfRecPkts = rte_ring_dequeue_burst(mTxRing, reinterpret_cast<void**>(txPackets), RX_BURST_SIZE, nullptr);
  if (nrOfRecPkts == 0) {
    return;
  }
  mEngine->sendPackets(txPackets, nrOfRecPkts);
}
