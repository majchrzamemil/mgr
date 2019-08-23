#ifndef PACKET_PROCESSOR
#define PACKET_PROCESSOR
#include <rte_ether.h>
#include <rte_ip.h>
#include <rte_tcp.h>
#include <rte_ring.h>

#include <memory>

#include "Http.hpp"
#include "Engine.hpp"

class PacketProcessor{
  public:
    PacketProcessor(rte_ring* rxRing, rte_ring* txRing){}
    void processPackets() {} //run
  private:
    ether_hdr* mEthHdr;
    ipv4_hdr* mIpHdr;
    tcp_hdr* mTcpHdr;
    Http* mHttp;

    rte_ring* mRxRing;
    rte_ring* mTxRing;
};
#endif
