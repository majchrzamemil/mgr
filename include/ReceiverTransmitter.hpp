#ifndef RECEIVER_TRANSMITTER
#define RECEIVER_TRANSMITTER
#include "DpdkEngine.hpp"
#include "rte_ring.h"

enum class EngineType {
  DPDK,
  SOCKET
};

class ReceiverTransmitter {
  public:
    ReceiverTransmitter(rte_ring* rxRing, rte_ring* txRing, EngineType type, int dpdkArgc, char** dpdkArgv);
    void run();
  private:
    void receivePackets();
    void sendPackets();
    std::unique_ptr<Engine> mEngine;
    rte_ring* mRxRing;
    rte_ring* mTxRing;
};
#endif
