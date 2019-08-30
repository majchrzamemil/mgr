#include "../include/DpdkEngine.hpp"

#include <rte_ethdev.h>
#include <rte_config.h>
#include <rte_bus_pci.h>

#include <arpa/inet.h>

#include <memory>
#include <iostream>

bool DpdkEngine::init(int dpdkArgc, char** dpdkArgv) {
  Engine::init(dpdkArgc, dpdkArgv);

  auto numOfPorts = rte_eth_dev_count_avail();
  if (numOfPorts == 1u) {
    mDevice = std::make_unique<DpdkDevice>(0, std::string("dupa"), 2047u, 64, 0);
  }

  return mDevice != nullptr;
}

void DpdkEngine::startEngine() {
  mDevice->startDevice();
}

uint16_t DpdkEngine::receivePackets(Packet** packets) {
  const auto devId = mDevice->getDeviceId();
  auto nrOfRecPkts = rte_eth_rx_burst(devId, 0, mRxPackets, RX_BURST_SIZE);
  if (unlikely(nrOfRecPkts == 0u)) {
    return 0u;
  }

  for (auto it{0u}; it < nrOfRecPkts; ++it) {
    packets[it] = new Packet(mRxPackets[it]);
  }

  return nrOfRecPkts;
}
void DpdkEngine::sendPackets(Packet** packets, uint16_t pktCount) {
  rte_mbuf* mBufsToSend[TX_BURST_SIZE];
//for now, on ring should be more than TX_BURST_SIZE
  pktCount = pktCount > TX_BURST_SIZE ? TX_BURST_SIZE : pktCount;
  for (auto it{0u}; it < pktCount; ++it) {
    mBufsToSend[it] = packets[it]->getMBuf();
    swapMac(mBufsToSend[it]);
    delete packets[it];
  }
  //create some constant for queuId
  auto nrSentPkts = rte_eth_tx_burst(mDevice->getDeviceId(), 0, mBufsToSend, pktCount);

  if (nrSentPkts != pktCount) {
    for (auto it{nrSentPkts}; it < pktCount; ++it) {
      rte_pktmbuf_free(mBufsToSend[it]);
    }
  }

}

void DpdkEngine::freePackets(rte_ring* freeRing) const {
  Packet* packets[TX_BURST_SIZE];
  auto nrOfPkts = rte_ring_dequeue_burst(freeRing, reinterpret_cast<void**>(packets), TX_BURST_SIZE, nullptr);
  if (nrOfPkts == 0u) {
    return;
  }
  for (auto it{0u}; it < nrOfPkts; ++it) {
    rte_pktmbuf_free(packets[it]->getMBuf());
  }
}

void DpdkEngine::swapMac(rte_mbuf* packet) const {
  ether_hdr*  etherHeader = rte_pktmbuf_mtod(packet, ether_hdr*);
  ether_addr d_addr = etherHeader->d_addr;
  etherHeader->d_addr = etherHeader->s_addr;
  etherHeader->s_addr = d_addr;
}
