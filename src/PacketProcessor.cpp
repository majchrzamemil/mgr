#include "../include/PacketProcessor.hpp"

#include<bitset>
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
      if (handleIpPacket(rxPackets[it])) {
        freePackets[pktsToFree++] = rxPackets[it];
        continue;
      }
      if (!handleTcpPacket()) {
        freePackets[pktsToFree++] = rxPackets[it];
        continue;
      }

      txPackets[pktsToSend++] = rxPackets[it];

    }
    rte_ring_enqueue_burst(mFreeRing, reinterpret_cast<void**>(freePackets), pktsToFree, nullptr);
    rte_ring_enqueue_burst(mTxRing, reinterpret_cast<void**>(txPackets), pktsToSend, nullptr);
  }
}

bool PacketProcessor::handleIpPacket(Packet* packet) {
  mIpHdr = reinterpret_cast<ipv4_hdr*>(packet->getData());
  constexpr uint8_t tcpProto{6u};
  if (mIpHdr->next_proto_id != tcpProto) {
    return false;
  }
  if (mIpHdr != nullptr) {
    uint32_t dstAddr = mIpHdr->dst_addr;
    mIpHdr->dst_addr = mIpHdr->src_addr;
    mIpHdr->src_addr = dstAddr;
  }
  return true;
}

bool PacketProcessor::handleTcpPacket() {
  constexpr uint8_t ipHdrSize{sizeof(ipv4_hdr)};
  constexpr uint16_t pshFlag{0x08};
  constexpr uint16_t ackFlag{0x10};
  constexpr uint16_t synFlag{0x02};
  constexpr uint16_t pshAckFlag{pshFlag | ackFlag};

  mTcpHdr = reinterpret_cast<tcp_hdr*>(reinterpret_cast<uint8_t*>(mIpHdr) + ipHdrSize);

  //PSH and ACK handle HTTP
  if (!(mTcpHdr->tcp_flags ^ pshAckFlag)) {
    handleHttpPacket();
  } else if (!(mTcpHdr->tcp_flags ^ synFlag)) { // SYN, send back SYN and ACK
    mTcpHdr->tcp_flags |= ackFlag;
    ++mTcpHdr->recv_ack;
  } else { //discard other packets, no support for now
    return false; 
  }
  swapPorts();
  return true;
}

void PacketProcessor::swapPorts() {
  const uint16_t dst_port{mTcpHdr->dst_port};
  mTcpHdr->dst_port = mTcpHdr->src_port;
  mTcpHdr->src_port = dst_port;
}
