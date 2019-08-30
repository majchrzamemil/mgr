#include "../include/ReceiverTransmitter.hpp"
#include "../include/DpdkEngine.hpp"

#include <rte_ether.h>
#include <rte_mbuf.h>

#include <iostream>


ReceiverTransmitter::ReceiverTransmitter(rte_ring* const rxRing, rte_ring* const txRing, rte_ring* const freeRing,
    Engine* const engine, const EngineConfig& config):  mEngine{engine}, mRxRing{rxRing}, mTxRing{txRing}, mFreeRing{freeRing},
  mRxBurstSize{config.rxBurstSize}, mTxBurstSize{config.txBurstSize}  {
    //free it somewhere
    rxPackets = new Packet*[mRxBurstSize]; 
  }

void ReceiverTransmitter::run() {
  while (true) {
    receivePackets();
    sendPackets();
    mEngine->freePackets(mFreeRing);

  }
}

void ReceiverTransmitter::receivePackets() {
  Packet*  packets[mRxBurstSize];
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
  Packet* txPackets[mTxBurstSize];
  auto nrOfRecPkts = rte_ring_dequeue_burst(mTxRing, reinterpret_cast<void**>(txPackets), mTxBurstSize, nullptr);
  if (nrOfRecPkts == 0) {
    return;
  }
  mEngine->sendPackets(txPackets, nrOfRecPkts);
}
