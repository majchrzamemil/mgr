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

//for now
int runRT(void *arg) {
  ReceiverTransmitter* rt = static_cast<ReceiverTransmitter*>(arg);
  rt->run();
  return 0;
}

int
main(int argc, char **argv) {
  unsigned lcore_id;

  std::unique_ptr<Engine> engine = std::make_unique<DpdkEngine>();

  engine->init(argc, argv);

  std::unique_ptr<rte_ring> rxRing(rte_ring_create("rxRing", RX_BURST_SIZE, SOCKET_ID_ANY, 0));
  std::unique_ptr<rte_ring> txRing(rte_ring_create("txRing", TX_BURST_SIZE, SOCKET_ID_ANY, 0));
  std::unique_ptr<rte_ring> freeRing(rte_ring_create("freeRing", TX_BURST_SIZE, SOCKET_ID_ANY, 0));

  std::unique_ptr<ReceiverTransmitter> rt = std::make_unique<ReceiverTransmitter>(rxRing.get(), txRing.get(),
      freeRing.get(), engine.get());

  engine->startEngine();
  RTE_LCORE_FOREACH_SLAVE(lcore_id) {
    rte_eal_remote_launch(runRT, rt.get(), lcore_id);
    if (rte_eal_wait_lcore(lcore_id) < 0) {
      break;
    }
  }
  return 0;
}
