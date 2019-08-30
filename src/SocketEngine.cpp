#include "../include/SocketEngine.hpp"

#include <arpa/inet.h>
#include <iostream>
#include <rte_ethdev.h>
//for now, later create init in Engine.hpp and there init DPDK EAL
bool SocketEngine::init([[maybe_unused]]int dpdkArgc, [[maybe_unused]]char** dpdkArgv) {
  Engine::init(dpdkArgc, dpdkArgv);

  mDevice = std::make_unique<SocketDevice>("enp0s25");
  mDevice->setIpAddr(std::string{"192.168.1.1"});
  return mDevice != nullptr;
}

//it should be bool, when starting device fails
void SocketEngine::startEngine() {
  mDevice->startDevice();
}

uint16_t SocketEngine::receivePackets(Packet** packets) {
  //move somewhere, think about buffer size, think about making this property
  constexpr uint32_t bufferSize{65536u};
  uint8_t* pktBuffer = new uint8_t[bufferSize];
  int packetSize = recv(mDevice->getSocketDesc(), pktBuffer, bufferSize, 0);
  if (packetSize == -1) {
    return 0u;
  }
  packets[0u] = new Packet(pktBuffer, packetSize);
  return 1u;
}

void SocketEngine::sendPackets(Packet** packets, [[maybe_unused]]uint16_t pktCount) {
  sockaddr_in dst_addr;

  dst_addr.sin_family = AF_INET;
  dst_addr.sin_port = htons(1000);
  dst_addr.sin_addr.s_addr = reinterpret_cast<ipv4_hdr*>(packets[0u]->getData())->dst_addr;
  //this method shouldn't be void
  [[maybe_unused]]int rt = sendto(mDevice->getSocketDesc(), packets[0u]->getData(), packets[0u]->getDataLen(),  0,
                                  (sockaddr*)&dst_addr, sizeof(sockaddr));
 // if (rt != -1) {
 //   packets[0u]->freeData();
 // }
  packets[0u]->freeData();
  delete packets[0u];
}

void SocketEngine::freePackets(rte_ring*) const {
}
