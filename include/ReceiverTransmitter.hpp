#ifndef RECEIVER_TRANSMITTER
#define RECEIVER_TRANSMITTER

#include "PacketProcessor.hpp"
#include "DpdkEngine.hpp"

#include <rte_ring.h>

class ReceiverTransmitter {
 public:
  //maybe struct for rings, to many parameters
  ReceiverTransmitter(rte_ring* const rxRing, rte_ring* const txRing, rte_ring* const freeRing,
                      Engine* const engine,
                      const EngineConfig& config);
  void run();
  ~ReceiverTransmitter();
 private:
  void receivePackets();
  void sendPackets();
  Engine* mEngine;
  rte_ring* const mRxRing;
  rte_ring* const mTxRing;
  rte_ring* const mFreeRing;
  const uint16_t mRxBurstSize;
  const uint16_t mTxBurstSize;
  Packet** rxPackets;
  PacketProcessor mPacketProcessor;
};
#endif
