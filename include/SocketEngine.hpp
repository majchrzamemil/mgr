#ifndef SOCKET_ENGINE
#define SOCKET_ENGINE

#include "../include/Engine.hpp"
#include "../include/SocketDevice.hpp"

#include <memory>

class SocketEngine : public Engine {
 public:
  bool init(int dpdkArgc, char** dpdkArgv) override;
  void startEngine() override;
  uint16_t receivePackets(Packet**) override;
  void sendPackets(Packet**, uint16_t pktCount) override;
  void freePackets(rte_ring* freeRing) const override;
 private:
  std::unique_ptr<SocketDevice> mDevice;
};

#endif
