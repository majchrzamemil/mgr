#ifndef DPDK_DEVICE
#define DPDK_DEVICE

#include <rte_mbuf.h>
#include <rte_mempool.h>
#include <rte_ether.h>

#include <cstdint>
#include <string>
#include <array>

#include <sys/socket.h>
#include <netinet/ip.h>

class DpdkDevice {
 public:
  DpdkDevice(const uint8_t portId, const uint32_t mBufPollSize,
             const uint16_t memPoolCashSize, const uint8_t memPoolFlags, uint16_t rxBurstSize, uint16_t txBurstSize);
  [[nodiscard]] uint8_t getDeviceId() const {
    return mPortId;
  }
  [[nodiscard]]ether_addr getMacAddress() const {
    return mMacAddress;
  }

  [[nodiscard]] rte_mempool* getMempool() const {
    return mMemPool;
  }
  bool startDevice() const;

  bool setIpAddr(std::string &ipStr);
 private:
  ether_addr mMacAddress;
  uint8_t mPortId;
  rte_mempool* mMemPool;
  in_addr mIpAddr;
};

#endif
