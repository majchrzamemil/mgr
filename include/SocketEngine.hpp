#ifndef SOCKET_ENGINE
#define SOCKET_ENGINE

#include "../include/Engine.hpp"
#include "../include/SocketDevice.hpp"

#include <memory>

class SocketEngine : public Engine {
 public:
  bool init(int dpdkArgc, char** dpdkArgv, const EngineConfig& config) override;
  bool startEngine() override;
  uint16_t receivePackets(Packet**) override;
  bool sendPackets(Packet**, uint16_t pktCount) override;
  void freePackets(rte_ring* freeRing) const override;
 private:
  constexpr static uint32_t bufferSize{65536u};
  std::unique_ptr<SocketDevice> mDevice;
};

#endif
