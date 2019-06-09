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
  std::string ringMode{"ring_mp_mc"};
  rte_mempool_set_ops_byname(memPool, ringMode.c_str(), nullptr);
  rte_pktmbuf_pool_init(memPool, nullptr);
  rte_mempool_populate_default(memPool);
  rte_mempool_obj_iter(memPool, rte_pktmbuf_init, nullptr);

  //queue
  struct rte_eth_dev_info dev_info;
  rte_eth_dev_info_get(portId, &dev_info);

struct rte_eth_conf portConf;
  memset(&portConf, 0, sizeof(rte_eth_conf));
  portConf.rxmode.split_hdr_size = 0;
  portConf.rxmode.max_rx_pkt_len = 1500;
  portConf.rxmode.mq_mode = ETH_MQ_RX_NONE;
  portConf.rxmode.offloads |= dev_info.rx_offload_capa;
  portConf.txmode.offloads |= dev_info.tx_offload_capa & DEV_TX_OFFLOAD_MBUF_FAST_FREE;
  portConf.txmode.hw_vlan_reject_tagged = 0;
  portConf.txmode.hw_vlan_reject_untagged = 0;
  portConf.txmode.hw_vlan_insert_pvid = 0;
  portConf.txmode.mq_mode = ETH_MQ_TX_NONE;
  rte_eth_dev_configure(portId, 1, 1, &portConf);

  if (auto ret = rte_eth_rx_queue_setup(portId, 0, 64, rte_eth_dev_socket_id(portId), nullptr, memPool) < 0) {
          rte_exit(EXIT_FAILURE, "rte_eth_rx_queue_setup:err=%d, port=%u\n",
          ret, portId);
  }
  
  if (auto ret = rte_eth_tx_queue_setup(portId, 0, 32, SOCKET_ID_ANY/*rte_eth_dev_socket_id(portId)*/, nullptr) < 0) {
          rte_exit(EXIT_FAILURE, "rte_eth_tx_queue_setup:err=%d, port=%u\n",
          ret, portId);
  }
}

bool DpdkDevice::startDevice() const {
  if(rte_eth_dev_start(portId) < 0) {
    return false; 
  }
 
  rte_eth_promiscuous_enable(portId);
  return true;
}

bool DpdkDevice::setIpAddr(std::string &ipStr) {
  return (inet_pton(AF_INET, ipStr.c_str(), reinterpret_cast<void*>(&ipAddr)) == 1) ? true : false;
}
