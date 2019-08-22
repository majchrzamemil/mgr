#include <rte_ether.h>
#include <rte_ip.h>
#include <rte_tcp.h>
#include <rte_ring.h>

#include "Http.hpp"

class PacketProcessor{
  public:
    PacketProcessor() {}
    void processPackets() {} //run
  private:
    ether_hdr* mEthHdr;
    ipv4_hdr* mIpHdr;
    tcp_hdr* mTcpHdr;
    Http* mHttp;

    rte_ring* rxRing;
    rte_ring* txRing;
};
