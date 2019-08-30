#include "../include/PacketProcessor.hpp"
#include <iostream>

//for test remove
//#include<netinet/ip.h>
//#include <arpa/inet.h>

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
      handleIpPacket(rxPackets[it]);

      if (mIpHdr == nullptr) {
        freePackets[pktsToFree++] = rxPackets[it];
        continue;
      }
      handleTcpPacket();
      txPackets[pktsToSend++] = rxPackets[it];

    }
    rte_ring_enqueue_burst(mFreeRing, reinterpret_cast<void**>(freePackets), pktsToFree, nullptr);
    rte_ring_enqueue_burst(mTxRing, reinterpret_cast<void**>(txPackets), pktsToSend, nullptr);
  }
}

void PacketProcessor::handleIpPacket(Packet* packet) {
  mIpHdr = reinterpret_cast<ipv4_hdr*>(packet->getData());
  if (mIpHdr != nullptr) {
    uint32_t dstAddr = mIpHdr->dst_addr;
    mIpHdr->dst_addr = mIpHdr->src_addr;
    mIpHdr->src_addr = dstAddr;
  }
//  ipv4_hdr* ip = reinterpret_cast<ipv4_hdr*>(packet->getData());
//  std::cout << "Destination adddres after swap: " << htonl(ip->dst_addr) << std::endl;
//  struct sockaddr_in source_socket_address, dest_socket_address;
//    iphdr *ip_packet = (struct iphdr *)packet->getData();
////
//    memset(&source_socket_address, 0, sizeof(source_socket_address));
//    source_socket_address.sin_addr.s_addr = ip_packet->saddr;
//    memset(&dest_socket_address, 0, sizeof(dest_socket_address));
//    dest_socket_address.sin_addr.s_addr = ip_packet->daddr;
//
//    ip_packet->daddr = ip_packet->saddr;
//    ip_packet->saddr = dest_socket_address.sin_addr.s_addr;
//
//  printf("Incoming Packet: \n");
//  printf("Packet Size (bytes): %d\n", ntohs(ip_packet->tot_len));
//  printf("Destination Address: %s\n", (char *)inet_ntoa(dest_socket_address.sin_addr));
//  printf("Identification: %d\n\n", ntohs(ip_packet->id));
}
