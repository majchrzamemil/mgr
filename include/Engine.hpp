#ifndef ENGINE
#define ENGINE
#include <cstdint>

#include "Packet.hpp"

class Engine {
  public:
    virtual void startEngine() = 0;

    virtual uint16_t receivePackets(Packet** /*think about collection*/) = 0;

    virtual void sendPackets(const Packet**, uint16_t pktCount) = 0;

    virtual bool init(int dpdkArgc, char** dpdkArgv) = 0;

    virtual void freePackets(rte_ring* freeRing) = 0;

  protected:
};
#endif
