#ifndef DPDK_DEVICE
#define DPDK_DEVICE

#include <rte_mbuf.h>
#include <rte_mempool.h>

#include <cstdint>
#include <string>
#include <array>
#include <sys/socket.h>
#include <netinet/ip.h>

using MacAddress = std::array<uint8_t, 6>;

constexpr uint16_t RX_BURST_SIZE{64u};
constexpr uint16_t TX_BURST_SIZE{32u};

class DpdkDevice {
 public:
  DpdkDevice(const uint8_t portId, const std::string& pciAddr, const uint32_t mBufPollSize,
             const uint16_t memPoolCashSize, const uint8_t memPoolFlags);
  [[nodiscard]] uint8_t getDeviceId() const {
    return mPortId;
  }

  [[nodiscard]]MacAddress getMacAddress() const {
    return mMacAddress;
  }

  [[nodiscard]] std::string getPciAddress() const {
    return mPciAddress;
  }

  [[nodiscard]] rte_mempool* getMempool() const {
    return mMemPool;
  }
  bool startDevice() const;

  bool setIpAddr(std::string &ipStr);
 private:
  std::string mPciAddress;
  uint8_t mPortId;
  MacAddress mMacAddress;
  rte_mempool* mMemPool;
  in_addr mIpAddr;
};

#endif
