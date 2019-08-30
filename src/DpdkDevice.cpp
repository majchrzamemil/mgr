#include "../include/DpdkDevice.hpp"

#include <arpa/inet.h>

#include <rte_ethdev.h>
#include <rte_config.h>
//split to smaller functions
DpdkDevice::DpdkDevice(const uint8_t portId, const uint32_t mBufPollSize,
                       const uint16_t memPoolCashSize, const uint8_t memPoolFlags, uint16_t rxBurstSize, uint16_t txBurstSize) :  mPortId{portId} {
  ether_addr macAddr;
  rte_eth_macaddr_get(portId, &macAddr);
  std::copy(std::begin(macAddr.addr_bytes), std::end(macAddr.addr_bytes), std::begin(mMacAddress.addr_bytes));
  //init of mempool
  mMemPool = rte_mempool_create_empty("mMemPool", mBufPollSize, RTE_MBUF_DEFAULT_BUF_SIZE, memPoolCashSize,
                                      sizeof(rte_pktmbuf_pool_private), SOCKET_ID_ANY, MEMPOOL_F_SP_PUT | MEMPOOL_F_SC_GET);

  std::string ringMode{"ring_sp_sc"};
  rte_mempool_set_ops_byname(mMemPool, ringMode.c_str(), nullptr);
  rte_pktmbuf_pool_init(mMemPool, nullptr);
  rte_mempool_populate_default(mMemPool);
  rte_mempool_obj_iter(mMemPool, rte_pktmbuf_init, nullptr);

  //queue
  struct rte_eth_dev_info dev_info;
  rte_eth_dev_info_get(portId, &dev_info);

  struct rte_eth_conf portConf;
  memset(&portConf, 0, sizeof(rte_eth_conf));
  portConf.rxmode.split_hdr_size = 0;
  portConf.rxmode.max_rx_pkt_len = 1500;
  portConf.rxmode.mq_mode = ETH_MQ_RX_NONE;
  portConf.rxmode.offloads |= dev_info.rx_offload_capa;
  portConf.txmode.offloads |= dev_info.tx_offload_capa;
  portConf.txmode.hw_vlan_reject_tagged = 0;
  portConf.txmode.hw_vlan_reject_untagged = 0;
  portConf.txmode.hw_vlan_insert_pvid = 0;
  portConf.txmode.mq_mode = ETH_MQ_TX_NONE;

  constexpr uint16_t nrOfQueues{1u};
  rte_eth_dev_configure(portId, nrOfQueues, nrOfQueues, &portConf);

  constexpr uint16_t queueId{0u};
  if (auto ret = rte_eth_rx_queue_setup(portId, queueId, rxBurstSize, rte_eth_dev_socket_id(portId), nullptr,
                                        mMemPool) < 0) {
    rte_exit(EXIT_FAILURE, "rte_eth_rx_queue_setup:err=%d, port=%u\n",
             ret, portId);
  }

  if (auto ret = rte_eth_tx_queue_setup(portId, queueId, txBurstSize, SOCKET_ID_ANY, nullptr) < 0) {
    rte_exit(EXIT_FAILURE, "rte_eth_tx_queue_setup:err=%d, port=%u\n",
             ret, portId);
  }
//   rte_eth_stats eth_stats;
//   auto rt = rte_eth_stats_get(0, &eth_stats);
//  if(rt  ==0)
//   printf("Opackets: %lu, oerrors: %lu\n", eth_stats.opackets, eth_stats.oerrors);
}

bool DpdkDevice::startDevice() const {
  if (rte_eth_dev_start(mPortId) < 0) {
    return false;
  }

  rte_eth_promiscuous_enable(mPortId);
  return true;
}

bool DpdkDevice::setIpAddr(std::string &ipStr) {
  return (inet_pton(AF_INET, ipStr.c_str(), reinterpret_cast<void*>(&mIpAddr)) == 1) ? true : false;
}
