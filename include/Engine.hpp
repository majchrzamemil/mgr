#ifndef ENGINE
#define ENGINE
#include <cstdint>
#include <rte_ether.h>

#include <string>

#include "Packet.hpp"

struct EngineConfig {
  uint16_t portId;
  uint32_t mBuffPoolSize;
  uint16_t memPoolCashSize;
  uint8_t memPoolFlags;
  std::string ipAddr;
  uint16_t tcpPort;
  uint16_t txBurstSize;
  uint16_t rxBurstSize;
  std::string devName;
};

class Engine {
 public:
  virtual bool startEngine() = 0;
  virtual uint16_t receivePackets(Packet**) = 0;
  virtual bool sendPackets(Packet**, const uint16_t pktCount) = 0;
  virtual bool init(int dpdkArgc, char** dpdkArgv, const EngineConfig& config);
  virtual void freePackets(rte_ring* freeRing) const = 0;
  virtual ~Engine() = default;
};
#endif
