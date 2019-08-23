#ifndef ENGINE
#define ENGINE
#include <cstdint>

#include <rte_ether.h>

class Engine {
  public:
    virtual void startEngine() = 0;

    virtual uint16_t receivePackets(ether_hdr** /*think about collection*/) = 0;

    virtual void sendPackets(const ether_hdr**, uint16_t pktCount) = 0;

    virtual bool init(int dpdkArgc, char** dpdkArgv) = 0; 
};
#endif
