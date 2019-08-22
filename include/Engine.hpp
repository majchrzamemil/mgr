#include <cstdint>

#include "Http.hpp"
class Engine {
  public:
    virtual void startEngine() = 0;

    virtual uint8_t receivePackets(Http** /*think about collection*/) = 0;

    virtual void sendPackets(const Http**, uint8_t pktCount) = 0;
    
};
