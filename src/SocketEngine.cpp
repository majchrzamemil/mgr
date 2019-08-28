#include "../include/SocketEngine.hpp"

#include <arpa/inet.h>

//for now, later create init in Engine.hpp and there init DPDK EAL
bool SocketEngine::init([[maybe_unused]]int dpdkArgc, [[maybe_unused]]char** dpdkArgv) {

  //later add this to init parameter
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
//for now debug, later move this
  struct sockaddr_in source_socket_address, dest_socket_address;
  sockaddr_in dst_addr;
  struct iphdr *ip_packet = (struct iphdr *)packets[0]->getData();

  memset(&source_socket_address, 0, sizeof(source_socket_address));
  source_socket_address.sin_addr.s_addr = ip_packet->saddr;
  memset(&dest_socket_address, 0, sizeof(dest_socket_address));
  dest_socket_address.sin_addr.s_addr = ip_packet->daddr;

  ip_packet->saddr = dest_socket_address.sin_addr.s_addr;
  ip_packet->daddr = source_socket_address.sin_addr.s_addr;
  dst_addr.sin_family = AF_INET;
  dst_addr.sin_port = htons(1000);
  dst_addr.sin_addr.s_addr = ip_packet->daddr;

//  printf("Incoming Packet: \n");
//  printf("Packet Size (bytes): %d\n", ntohs(ip_packet->tot_len));
//  printf("Source Address: %s\n", (char *)inet_ntoa(source_socket_address.sin_addr));
//  printf("Destination Address: %s\n", (char *)inet_ntoa(dest_socket_address.sin_addr));
//  printf("Sending to Address: %s\n", (char *)inet_ntoa(mDevice->getIpAddr()));
//  printf("Identification: %d\n\n", ntohs(ip_packet->id));
  //this method shouldn't be void
  [[maybe_unused]]int rt = sendto(mDevice->getSocketDesc(), packets[0u]->getData(), packets[0u]->getDataLen(),  0,
                                  (sockaddr*)&dst_addr, sizeof(sockaddr));
}

void SocketEngine::freePackets(rte_ring*) const {
}
