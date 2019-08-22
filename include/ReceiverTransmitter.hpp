#include "DpdkEngine.hpp"
#include "rte_ring.h"

class ReceiverTransmitter {
  public:
    ReceiverTransmitter(Engine* engine) : mEngine{std::make_unique<Engine>(engine)}, txRing{nullptr}, rxRing{nullptr} {} 
    void run() {};
  private:
    void receivePackets() {};
    void sendPackets() {};
    std::unique_ptr<Engine> mEngine;
    rte_ring* txRing;
    rte_ring* rxRing; //think about shared_ptr
};
