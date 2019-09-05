#ifndef PACKET_PROCESSOR
#define PACKET_PROCESSOR
#include <rte_ether.h>
#include <rte_ip.h>
#include <rte_tcp.h>
#include <rte_ring.h>

#include <memory>

#include "Engine.hpp"
#include "HttpRequest.hpp"
#include "HttpResponse.hpp"

class PacketProcessor {
 public:
  PacketProcessor() = default;
  HttpRequest* processPacket(Packet* packet);
  Packet* processHttpResp(HttpResponse* response);
  ~PacketProcessor() {}
 private:
  bool handleIpPacket(Packet* packet);
  bool handleTcpPacket();
  bool isHttpNextLayer();
  void swapPorts();
  void prepareOutputIpPacket();
  Packet* mPacket;
  ipv4_hdr* mIpHdr;
  tcp_hdr* mTcpHdr;
};
#endif
