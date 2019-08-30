#include "../include/SocketEngine.hpp"

#include <arpa/inet.h>
#include <iostream>
#include <rte_ethdev.h>

bool SocketEngine::init(int dpdkArgc, char** dpdkArgv, const EngineConfig& config) {
  Engine::init(dpdkArgc, dpdkArgv, config);

  mDevice = std::make_unique<SocketDevice>(config.devName);
  mDevice->setIpAddr(config.ipAddr);
  return mDevice != nullptr;
}

bool SocketEngine::startEngine() {
  return mDevice->startDevice();
}

uint16_t SocketEngine::receivePackets(Packet** packets) {

  uint8_t* pktBuffer = new uint8_t[bufferSize];
  int packetSize = recv(mDevice->getSocketDesc(), pktBuffer, bufferSize, 0);
  if (packetSize == -1) {
    return 0u;
  }
  packets[0u] = new Packet(pktBuffer, packetSize);
  return 1u;
}

bool SocketEngine::sendPackets(Packet** packets, [[maybe_unused]]uint16_t pktCount) {
  sockaddr_in dst_addr;

  dst_addr.sin_family = AF_INET;
  //after changing to tcp make constant for port readed from config
  dst_addr.sin_port = htons(1000);
  dst_addr.sin_addr.s_addr = reinterpret_cast<ipv4_hdr*>(packets[0u]->getData())->dst_addr;
  
  int rt = sendto(mDevice->getSocketDesc(), packets[0u]->getData(), packets[0u]->getDataLen(),  0,
                                  (sockaddr*)&dst_addr, sizeof(sockaddr));

  packets[0u]->freeData();
  delete packets[0u];
  if (rt != -1) {
    return false;
  }
  return true;
}

void SocketEngine::freePackets(rte_ring* freeRing) const {
  Packet* packets[32u];
  auto nrOfPkts = rte_ring_dequeue_burst(freeRing, reinterpret_cast<void**>(packets), 32u, nullptr);
  if (nrOfPkts == 0u) {
    return;
  }
  for (auto it{0u}; it < nrOfPkts; ++it) {
    std::cout <<"free";
    packets[it]->freeData(); 
    delete packets[it];
  }
}
