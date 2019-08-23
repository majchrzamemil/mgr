#ifndef DPDK_ENGINE
#define DPDK_ENGINE
#include "../include/DpdkDevice.hpp"
#include "../include/Engine.hpp"

#include <rte_mbuf.h>

#include <memory>

class DpdkEngine : public Engine {
  public:
    bool init(int dpdkArgc, char** dpdkArgv);
    void startEngine();
    uint16_t receivePackets(ether_hdr**);
    void sendPackets(const ether_hdr**, uint16_t pktCount);
  private:
    std::unique_ptr<DpdkDevice> device;
    rte_mbuf* rxPackets[RX_BURST_SIZE];
    rte_mbuf* txPackets[TX_BURST_SIZE];
};
#endif
