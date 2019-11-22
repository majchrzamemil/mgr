#ifndef DPDK_ENGINE
#define DPDK_ENGINE
#include "../include/DpdkDevice.hpp"
#include "../include/Engine.hpp"

#include <rte_mbuf.h>

#include <memory>

class DpdkEngine : public Engine {
 public:
  bool init(int dpdkArgc, char** dpdkArgv, const EngineConfig& config) override;
  bool startEngine() override;
  [[nodiscard]]uint16_t receivePackets(Packet**) override;
  bool sendPackets(Packet**, uint16_t pktCount) override;
  void freePackets(rte_ring* freeRing) const override;
  ~DpdkEngine();
 private:
  uint16_t mRxBurstSize;
  uint16_t mTxBurstSize;
  void swapMac(rte_mbuf* packet) const;
  std::unique_ptr<DpdkDevice> mDevice;
  rte_mbuf** mRxPackets;
  rte_mbuf* mBufsToSend[128];
  constexpr static uint16_t mQueueId{0u};
  // rte_mbuf* mTxPackets[TX_BURST_SIZE]; // you might check if this will work, without allocatig packets in sendPackets()
};
#endif
