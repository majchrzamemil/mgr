#ifndef RECEIVER_TRANSMITTER
#define RECEIVER_TRANSMITTER
#include "DpdkEngine.hpp"
#include "rte_ring.h"

class ReceiverTransmitter {
  public:
    ReceiverTransmitter(rte_ring* const rxRing, rte_ring* const txRing, rte_ring* const freeRing, Engine* const engine);
    void run();
    ~ReceiverTransmitter(){}
  private:
    void receivePackets();
    void sendPackets();
    Engine* mEngine;
    rte_ring* const mRxRing;
    rte_ring* const mTxRing;
    rte_ring* const mFreeRing;
    Packet* rxPackets[RX_BURST_SIZE];
};
#endif
