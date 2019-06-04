#include "../include/DpdkDevice.hpp"

#include <arpa/inet.h>

#include <rte_ethdev.h>
#include <rte_config.h>

#include <iostream>
DpdkDevice::DpdkDevice(const uint8_t portId, const std::string pciAddr, const uint32_t mBufPollSize,
                       const uint16_t memPoolCashSize, const uint8_t memPoolFlags) : portId{portId}, pciAddress{pciAddr} {
  ether_addr macAddr;
  rte_eth_macaddr_get(portId, &macAddr);
  std::copy(std::begin(macAddr.addr_bytes), std::end(macAddr.addr_bytes), std::begin(macAddress));
  //init of mempool
  memPool = rte_mempool_create_empty("memPool", mBufPollSize, RTE_MBUF_DEFAULT_BUF_SIZE, memPoolCashSize,
                                     sizeof(rte_pktmbuf_pool_private), 0, memPoolFlags);
  if (memPool == nullptr) {
    std::cout << "null\n"; 
  } 
  std::string ringMode{"ring_sp_sc"};
  rte_mempool_set_ops_byname(memPool, ringMode.c_str(), nullptr);
  rte_pktmbuf_pool_init(memPool, nullptr);
  rte_mempool_obj_iter(memPool, rte_pktmbuf_init, nullptr);

  //queue

  struct rte_eth_conf port_conf;
  port_conf.rxmode.split_hdr_size = 0;
//  port_conf.txmode.mq_mode = ETH_MQ_TX_NONE;
  port_conf.txmode.offloads =0x800f ;// DEV_TX_OFFLOAD_MBUF_FAST_FREE;

  rte_eth_dev_configure(portId, 1, 1, &port_conf);
  auto ret = rte_eth_rx_queue_setup(portId, 0, 1024,
               rte_eth_dev_socket_id(portId),
               nullptr,
               memPool);
  if (ret < 0) {
          rte_exit(EXIT_FAILURE, "rte_eth_rx_queue_setup:err=%d, port=%u\n",
          ret, portId);

    std::cout <<"cannot init queue\n"; 
  }
}

bool DpdkDevice::startDevice() const {
  rte_eth_promiscuous_enable(portId);
  return (rte_eth_dev_start(portId) < 0) ? false : true;
}

bool DpdkDevice::setIpAddr(std::string &ipStr) {
  return (inet_pton(AF_INET, ipStr.c_str(), reinterpret_cast<void*>(&ipAddr)) == 1) ? true : false;
}
