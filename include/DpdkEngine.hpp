#ifndef DPDK_ENGINE
#define DPDK_ENGINE
#include "../include/DpdkDevice.hpp"
#include "../include/Engine.hpp"

#include <rte_mbuf.h>

#include <memory>

class DpdkEngine : public Engine {
 public:
  bool init(int dpdkArgc, char** dpdkArgv) override;
  void startEngine() override;
  uint16_t receivePackets(Packet**) override;
  void sendPackets(Packet**, uint16_t pktCount) override;
  void freePackets(rte_ring* freeRing) const override;
 private:
  std::unique_ptr<DpdkDevice> mDevice;
  rte_mbuf* mRxPackets[RX_BURST_SIZE];
  // rte_mbuf* mTxPackets[TX_BURST_SIZE]; // you might check if this will work, without allocatig packets in sendPackets()
};
#endif
