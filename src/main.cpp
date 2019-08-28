#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <errno.h>
#include <sys/queue.h>
#include <iostream>

#include <rte_memory.h>
#include <rte_launch.h>
#include <rte_eal.h>
#include <rte_per_lcore.h>
#include <rte_lcore.h>
#include <rte_debug.h>
#include <rte_ethdev.h>

#include "../include/PacketProcessor.hpp"
#include "../include/ReceiverTransmitter.hpp"
#include "../include/Config.hpp"
#include "../include/DpdkEngine.hpp"
#include "../include/SocketEngine.hpp"

//for now
int runRT(void *arg) {
  ReceiverTransmitter* rt = static_cast<ReceiverTransmitter*>(arg);
  rt->run();
  return 0;
}

int runPacketProcessor(void * arg) {
  PacketProcessor* rt = static_cast<PacketProcessor*>(arg);
  rt->processPackets();
  return 0;
}

int
main(int argc, char **argv) {
//  unsigned lcore_id;

//  std::unique_ptr<Engine> engine = std::make_unique<DpdkEngine>();

  std::unique_ptr<Engine> engine = std::make_unique<SocketEngine>();
  engine->init(argc, argv);

  std::unique_ptr<rte_ring> rxRing(rte_ring_create("rxRing", RING_SIZE, SOCKET_ID_ANY, 0));
  std::unique_ptr<rte_ring> txRing(rte_ring_create("txRing", RING_SIZE, SOCKET_ID_ANY, 0));
  std::unique_ptr<rte_ring> freeRing(rte_ring_create("freeRing", RING_SIZE, SOCKET_ID_ANY, 0));

  std::unique_ptr<ReceiverTransmitter> rt = std::make_unique<ReceiverTransmitter>(rxRing.get(), txRing.get(),
      freeRing.get(), engine.get());
  std::unique_ptr<PacketProcessor> packetProcessor = std::make_unique<PacketProcessor>(rxRing.get(), txRing.get(),
      freeRing.get(), RX_BURST_SIZE);

  engine->startEngine();

  rt->run();
  //init those thread properly, remove magic lcore numbers
//  rte_eal_remote_launch(runPacketProcessor, packetProcessor.get(), RT_LCORE);
//  rte_eal_remote_launch(runRT, rt.get(), PP_LCORE);
//
//  RTE_LCORE_FOREACH_SLAVE(lcore_id) {
//    if (rte_eal_wait_lcore(lcore_id) < 0) {
//      break;
//    }
//  }
  return 0;
}
