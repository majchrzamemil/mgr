#include "../include/DpdkEngine.hpp"

#include <rte_ethdev.h>
#include <rte_config.h>
#include <rte_bus_pci.h>

#include <arpa/inet.h>

#include <memory>
#include <iostream>

bool DpdkEngine::init(int dpdkArgc, char** dpdkArgv, const EngineConfig& config) {
  Engine::init(dpdkArgc, dpdkArgv, config);

  mTxBurstSize = config.txBurstSize;
  mRxBurstSize = config.rxBurstSize;

  //free it somewhere
  mRxPackets = new rte_mbuf*[mRxBurstSize];

  auto numOfPorts = rte_eth_dev_count_avail();
  if (numOfPorts == 1u) {
    mDevice = std::make_unique<DpdkDevice>(config.portId, config.mBuffPoolSize, config. memPoolCashSize,
                                           config.memPoolFlags, config.rxBurstSize, config.txBurstSize);
  }

  return mDevice != nullptr;
}

bool DpdkEngine::startEngine() {
  return mDevice->startDevice();
}

uint16_t DpdkEngine::receivePackets(Packet** packets) {
  const auto devId = mDevice->getDeviceId();
  auto nrOfRecPkts = rte_eth_rx_burst(devId, mQueueId, mRxPackets, mRxBurstSize);
  if (unlikely(nrOfRecPkts == 0u)) {
    return 0u;
  }

  for (auto it{0u}; it < nrOfRecPkts; ++it) {
    packets[it] = new Packet(mRxPackets[it]);
  }

  return nrOfRecPkts;
}
bool DpdkEngine::sendPackets(Packet** packets, uint16_t pktCount) {
  rte_mbuf* mBufsToSend[mTxBurstSize];

  pktCount = pktCount > mTxBurstSize ? mTxBurstSize : pktCount;
  for (auto it{0u}; it < pktCount; ++it) {
    mBufsToSend[it] = packets[it]->getMBuf();
    swapMac(mBufsToSend[it]);
    const size_t mBufDataLen{packets[it]->getMBuf()->data_len};
    const size_t packetDataLen{packets[it]->getDataLen() + sizeof(ether_hdr)};
    if(mBufDataLen < packetDataLen) {
     [[maybe_unused]] auto ret = rte_pktmbuf_append(mBufsToSend[it], packetDataLen - mBufDataLen);
//      std::cout << "ret: " << ret << "appended: " << packetDataLen - mBufDataLen << "packet data len: " << packetDataLen << std::endl;
    }else if(mBufDataLen > packetDataLen) {
      rte_pktmbuf_trim(mBufsToSend[it], mBufDataLen - packetDataLen);
    }
    delete packets[it];
  }
  
  auto nrSentPkts = rte_eth_tx_burst(mDevice->getDeviceId(), mQueueId, mBufsToSend, pktCount);

  if (nrSentPkts != pktCount) {
    for (auto it{nrSentPkts}; it < pktCount; ++it) {
      rte_pktmbuf_free(mBufsToSend[it]);
    }
  }

}

void DpdkEngine::freePackets(rte_ring* freeRing) const {
  Packet* packets[mTxBurstSize];
  auto nrOfPkts = rte_ring_dequeue_burst(freeRing, reinterpret_cast<void**>(packets), mTxBurstSize, nullptr);
  if (nrOfPkts == 0u) {
    return;
  }
  for (auto it{0u}; it < nrOfPkts; ++it) {
    rte_pktmbuf_free(packets[it]->getMBuf());
    delete packets[it];
  }
}

void DpdkEngine::swapMac(rte_mbuf* packet) const {
  ether_hdr* etherHeader = rte_pktmbuf_mtod(packet, ether_hdr*);
  ether_addr d_addr = etherHeader->d_addr;
  etherHeader->d_addr = etherHeader->s_addr;
  etherHeader->s_addr = d_addr;
}
