#ifndef ENGINE
#define ENGINE
#include <cstdint>
#include <rte_ether.h>

#include "Packet.hpp"

class Engine {
 public:
  virtual void startEngine() = 0;
  virtual uint16_t receivePackets(Packet**) = 0;
  virtual void sendPackets(Packet**, const uint16_t pktCount) = 0;
  virtual bool init(int dpdkArgc, char** dpdkArgv) {
    if (rte_eal_init(dpdkArgc, dpdkArgv) < 0) {
      return false;
    }
  }
  virtual void freePackets(rte_ring* freeRing) const = 0;
 protected:
  //move to cpp
  //what should be here??
};
#endif
