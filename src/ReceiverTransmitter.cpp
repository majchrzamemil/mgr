#include "../include/ReceiverTransmitter.hpp"
#include "../include/DpdkEngine.hpp"

#include <rte_ether.h>
#include <rte_mbuf.h>

#include <iostream>

ReceiverTransmitter::ReceiverTransmitter(rte_ring* const rxRing, rte_ring* const txRing,
    rte_ring* const freeRing,
    Engine* const engine, const EngineConfig& config):  mEngine{engine}, mRxRing{rxRing}, mTxRing{txRing},
  mFreeRing{freeRing},
  mRxBurstSize{config.rxBurstSize}, mTxBurstSize{config.txBurstSize}  {
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
  HttpRequest* requests[mRxBurstSize];
  uint16_t requestsToEnqueue{0u};
  auto nrOfRecPkts = mEngine->receivePackets(packets);
  if (nrOfRecPkts == 0u) {
    return;
  }
  for (auto it{0u}; it < nrOfRecPkts; ++it) {
    if (auto* request = mPacketProcessor.processPacket(packets[it])){
      requests[requestsToEnqueue++] = request;
    } else {
      rte_ring_enqueue(mFreeRing, packets[it]);
    }

  }
  auto rt = rte_ring_enqueue_burst(mRxRing, reinterpret_cast<void**>(requests), requestsToEnqueue,
                                   nullptr);
  if (rt != requestsToEnqueue) {
    for (auto it{rt}; it < requestsToEnqueue; ++it) {
      rte_ring_enqueue(mFreeRing, requests[it]->getPacket());
      delete requests[it];
    }
  }
}

void ReceiverTransmitter::sendPackets() {
  Packet* txPackets[mTxBurstSize];
  HttpResponse* responses[mTxBurstSize];
  uint16_t packetsToSend{0u};

  auto nrOfRecPkts = rte_ring_dequeue_burst(mTxRing, reinterpret_cast<void**>(responses),
                     mTxBurstSize, nullptr);

  if (nrOfRecPkts == 0) {
    return;
  }
  for (auto it{0u}; it < nrOfRecPkts; ++it) {
    if (Packet* packet = mPacketProcessor.processHttpResp(responses[it])) {
      txPackets[packetsToSend++] = packet;
    } else {
      rte_ring_enqueue(mFreeRing, responses[it]->getPacket());
    }
  }
  mEngine->sendPackets(txPackets, nrOfRecPkts);
}

ReceiverTransmitter::~ReceiverTransmitter() {
  delete rxPackets;
}
