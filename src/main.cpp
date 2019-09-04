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

#include "../include/ReceiverTransmitter.hpp"
#include "../include/Config.hpp"
#include "../include/DpdkEngine.hpp"
#include "../include/SocketEngine.hpp"
#include "../include/HttpServer.hpp"

//for now
int runRT(void *arg) {
  ReceiverTransmitter* rt = static_cast<ReceiverTransmitter*>(arg);
  rt->run();
  return 0;
}

int runHttpServer(void *arg) {
  HttpServer* server = static_cast<HttpServer*>(arg);
  server->run();
  return 0;
}
//int runPacketProcessor(void * arg) {
//  PacketProcessor* rt = static_cast<PacketProcessor*>(arg);
//  rt->processPackets();
//  return 0;
//}

int
main(int argc, char **argv) {
  unsigned lcore_id;

  std::unique_ptr<Engine> engine;
  if(ENGINE_TYPE == EngineType::DPDK) {
   engine = std::make_unique<DpdkEngine>();
  } else {
    engine = std::make_unique<SocketEngine>();
  }

  if(!engine->init(argc, argv, config)){
    std::cout <<"Failed to start engine!\n";
    return -1;
  }

  //HttpRequest Ring
  std::unique_ptr<rte_ring> rxRing(rte_ring_create("rxRing", RING_SIZE, SOCKET_ID_ANY, 0));
  //HttpResponse Ring 
  std::unique_ptr<rte_ring> txRing(rte_ring_create("txRing", RING_SIZE, SOCKET_ID_ANY, 0));
  //Packet ring
  std::unique_ptr<rte_ring> freeRing(rte_ring_create("freeRing", RING_SIZE, SOCKET_ID_ANY, 0));

  std::unique_ptr<ReceiverTransmitter> rt = std::make_unique<ReceiverTransmitter>(rxRing.get(), txRing.get(),
     freeRing.get(), engine.get(), config);
  std::unique_ptr<HttpServer> server = std::make_unique<HttpServer>(rxRing.get(), txRing.get(), freeRing.get(), config.txBurstSize);

  engine->startEngine();

  rte_eal_remote_launch(runHttpServer, server.get(), PP_LCORE);
  rte_eal_remote_launch(runRT, rt.get(), RT_LCORE);

  RTE_LCORE_FOREACH_SLAVE(lcore_id) {
    if (rte_eal_wait_lcore(lcore_id) < 0) {
      break;
    }
  }
  return 0;
}
