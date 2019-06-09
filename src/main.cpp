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


#include "../include/Config.hpp"
#include "../include/DpdkEngine.hpp"

int processPackets(void *) {
  std::cout << "processing packets\n";
  //for now:
  uint8_t portId{0u};
  const uint8_t burstSize{64u};
  while (true) {
    rte_mbuf* packets[burstSize];
    auto nrOfRecPackets = rte_eth_rx_burst(portId, 0, packets, burstSize);
    for (auto pktId{0u}; pktId < nrOfRecPackets; ++pktId) {
      ether_hdr *srcEth{rte_pktmbuf_mtod(packets[pktId], ether_hdr *)};
std::cout << "received packet from:" << unsigned(srcEth->s_addr.addr_bytes[0]) << ":"
          << unsigned(srcEth->s_addr.addr_bytes[1]) << ":"
          << unsigned(srcEth->s_addr.addr_bytes[2]) << ":"
          << unsigned(srcEth->s_addr.addr_bytes[3]) << ":"
          << unsigned(srcEth->s_addr.addr_bytes[4]) << ":"
          << unsigned(srcEth->s_addr.addr_bytes[5]) << "\n"
                ;
    }
  }
  return 0;
}

int
main(int argc, char **argv) {
  unsigned lcore_id;
  DpdkEngine engine;
  if (engine.initDpdk(argc, argv)) {
    engine.startEngine();
  }
  RTE_LCORE_FOREACH_SLAVE(lcore_id) {
    rte_eal_remote_launch(processPackets, NULL, lcore_id);
    if (rte_eal_wait_lcore(lcore_id) < 0) {
      break;
    }
  }
  return 0;
}
