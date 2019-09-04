#include "../include/PacketProcessor.hpp"

#include<bitset>
#include <iostream>
#include <cstring>

#include "../include/HttpParser.hpp"
//for test remove
//#include<netinet/ip.h>
//#include <arpa/inet.h>

HttpRequest* PacketProcessor::processPacket(Packet* packet) {
  if (!handleIpPacket(packet)) {
    return nullptr;
  }
  mPacket = packet;
  if (!handleTcpPacket()) {
    return nullptr;
  }
  //for now tcp syn not working, think about this
  if (!isHttpNextLayer()) {
    return nullptr;
  }

  constexpr size_t httpOffset = sizeof(ipv4_hdr) + sizeof(tcp_hdr);
  uint8_t* payload = mPacket->getData() + httpOffset;
  HttpRequest* request = new HttpRequest{mPacket};

  HttpParser::parseRequest(reinterpret_cast<char*>(payload), request);
  return request;
}
Packet* PacketProcessor::processHttpResp(HttpResponse* response) {
  mPacket = response->getPacket();
  constexpr size_t httpOffset = sizeof(ipv4_hdr) + sizeof(tcp_hdr);
  uint8_t* payload = mPacket->getData() + httpOffset;
  std::string responseString = HttpParser::parseResponse(response);

  const size_t httpReqLen = mPacket->getDataLen() - httpOffset;
  mPacket->setDataLen(mPacket->getDataLen() - httpReqLen + responseString.length());
  strcpy(reinterpret_cast<char*>(payload), responseString.c_str());
 
  //handle eth and IP
  swapPorts();
  prepareOutputIpPacket();
  
  delete response;
  return mPacket;
}

bool PacketProcessor::handleIpPacket(Packet* packet) {
  mIpHdr = reinterpret_cast<ipv4_hdr*>(packet->getData());
  constexpr uint8_t tcpProto{6u};
  if (mIpHdr->next_proto_id != tcpProto) {
    return false;
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
    return true;
  } else if (!(mTcpHdr->tcp_flags ^ synFlag)) { // SYN, send back SYN and ACK
    mTcpHdr->tcp_flags |= ackFlag;
    ++mTcpHdr->recv_ack;
  } else { //discard other packets, no support for now
    return false;
  }
  return true;
}

void PacketProcessor::swapPorts() {
  const uint16_t dst_port{mTcpHdr->dst_port};
  mTcpHdr->dst_port = mTcpHdr->src_port;
  mTcpHdr->src_port = dst_port;
}

bool PacketProcessor::isHttpNextLayer() {
  return true;
}

void PacketProcessor::prepareOutputIpPacket() {
  if (mIpHdr != nullptr) {
    uint32_t dstAddr = mIpHdr->dst_addr;
    mIpHdr->dst_addr = mIpHdr->src_addr;
    mIpHdr->src_addr = dstAddr;
    mIpHdr->total_length = htons(mPacket->getDataLen());
  }
}
