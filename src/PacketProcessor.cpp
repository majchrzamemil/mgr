#include "../include/PacketProcessor.hpp"
#include <iostream>

//Move somewhere
constexpr uint16_t TX_BURST_SIZE{32u};
void PacketProcessor::processPackets() {
  while (true) {
    Packet* rxPackets[TX_BURST_SIZE];
    auto nrOfRecPkts = rte_ring_dequeue_burst(mRxRing, reinterpret_cast<void**>(&rxPackets), TX_BURST_SIZE, nullptr);
    if (nrOfRecPkts == 0) {
      continue;
    }
    for (auto it{0u}; it < nrOfRecPkts; ++it) {
      ether_hdr* eth  = reinterpret_cast<ether_hdr*>(rxPackets[it]->getData());
      ether_addr d_addr = eth->d_addr;
      eth->d_addr = eth->s_addr;
      eth->s_addr = d_addr;
    }
    rte_ring_enqueue_burst(mTxRing, reinterpret_cast<void**>(rxPackets), nrOfRecPkts, nullptr);
  }
}
