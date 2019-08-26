#include "../include/PacketProcessor.hpp"
#include <iostream>

//Move somewhere
void PacketProcessor::processPackets() {
  while (true) {
    Packet* rxPackets[mRxBurstSize];
    Packet* freePackets[mRxBurstSize];
    uint16_t pktsToFree{0u};
    auto nrOfRecPkts = rte_ring_dequeue_burst(mRxRing, reinterpret_cast<void**>(&rxPackets), mRxBurstSize, nullptr);

    if (nrOfRecPkts == 0) {
      continue;
    }

    Packet* txPackets[mRxBurstSize];
    uint16_t pktsToSend{0u};

    for (auto it{0u}; it < nrOfRecPkts; ++it) {
      ether_hdr* etherHeader{handlePacket(rxPackets[it])};
      if (etherHeader == nullptr) {
        std::cout <<  "ether free\n";
        //freePackets[pktsToFree++] = rxPackets[it];
        continue;
      }
      txPackets[pktsToSend++] = rxPackets[it];

    }
    rte_ring_enqueue_burst(mFreeRing, reinterpret_cast<void**>(freePackets), pktsToFree, nullptr);
    rte_ring_enqueue_burst(mTxRing, reinterpret_cast<void**>(txPackets), pktsToSend, nullptr);
  }
}

//handle checking if correct MAC
ether_hdr* PacketProcessor::handlePacket(Packet* packet) {
  ether_hdr*  etherHeader  = reinterpret_cast<ether_hdr*>(packet->getData());
  ether_addr d_addr = etherHeader->d_addr;
  etherHeader->d_addr = etherHeader->s_addr;
  etherHeader->s_addr = d_addr;
  return etherHeader;
}
