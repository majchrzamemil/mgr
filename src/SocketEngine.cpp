#include "../include/SocketEngine.hpp"

#include <rte_ip.h>
#include <rte_tcp.h>

#include <arpa/inet.h>
#include <rte_ethdev.h>

bool SocketEngine::init(int dpdkArgc, char** dpdkArgv, const EngineConfig& config) {
  if (!Engine::init(dpdkArgc, dpdkArgv, config)) {
    return false;
  }

  mDevice = std::make_unique<SocketDevice>(config.devName);
  mDevice->setIpAddr(config.ipAddr);
  return mDevice != nullptr;
}

bool SocketEngine::startEngine() {
  return mDevice->startDevice();
}

uint16_t SocketEngine::receivePackets(Packet** packets) {
  constexpr uint8_t pktId{0u};
  uint8_t* pktBuffer = new uint8_t[bufferSize];
  int packetSize = recv(mDevice->getSocketDesc(), pktBuffer, bufferSize, 0);
  if (packetSize == -1) {
    return 0u;
  }
  packets[pktId] = new Packet(pktBuffer, packetSize);
  return 1u;
}

bool SocketEngine::sendPackets(Packet** packets, [[maybe_unused]]uint16_t pktCount) {
  constexpr uint8_t pktId{0u};
  constexpr uint8_t sizeOfEthIp{sizeof(ether_hdr) + sizeof(ipv4_hdr)};
  const uint16_t tcpPort = reinterpret_cast<tcp_hdr*>(packets[pktId]->getData() +
                           sizeOfEthIp)->dst_port;

  sockaddr_in dst_addr;
  dst_addr.sin_family = AF_INET;
  dst_addr.sin_port = htons(tcpPort);
  dst_addr.sin_addr.s_addr = reinterpret_cast<ipv4_hdr*>(packets[pktId]->getData())->dst_addr;

  int rt = sendto(mDevice->getSocketDesc(), packets[pktId]->getData(), packets[pktId]->getDataLen(),
                  0,
                  (sockaddr*)&dst_addr, sizeof(sockaddr));

  packets[pktId]->freeData();
  delete packets[pktId];
  if (rt != -1) {
    return false;
  }
  return true;
}

void SocketEngine::freePackets(rte_ring* freeRing) const {
  constexpr uint16_t burstSize{32u};
  Packet* packets[burstSize];
  auto nrOfPkts = rte_ring_dequeue_burst(freeRing, reinterpret_cast<void**>(packets), burstSize,
                                         nullptr);
  if (nrOfPkts == 0u) {
    return;
  }
  for (auto it{0u}; it < nrOfPkts; ++it) {
    packets[it]->freeData();
    delete packets[it];
  }
}
