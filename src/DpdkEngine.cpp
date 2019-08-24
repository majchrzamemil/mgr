#include "../include/DpdkEngine.hpp"

#include <rte_ethdev.h>
#include <rte_config.h>
#include <rte_bus_pci.h>

#include <memory>
#include <iostream>

bool DpdkEngine::init(int dpdkArgc, char** dpdkArgv) {
  if(rte_eal_init(dpdkArgc, dpdkArgv) < 0) {
    return false; 
  }
  auto numOfPorts = rte_eth_dev_count_avail();
  if (numOfPorts == 1u) {
    device = std::make_unique<DpdkDevice>(0, std::string("dupa"), 2048u, 64, 0);      
  }
  if (device == nullptr) {
    return false; 
  }
  return true;
}

void DpdkEngine::startEngine() {
  device->startDevice();
}
//hede will be Packet insted of ether_hdr, Packet processor will handle that
//add freeRing, on this ring packet process will enqueue Packets to free, and Engine is going to perform freeing
uint16_t DpdkEngine::receivePackets(Packet** packets) {
  const auto devId = device->getDeviceId();
  auto nrOfRecPkts = rte_eth_rx_burst(devId, 0, rxPackets, RX_BURST_SIZE);
  if (unlikely(nrOfRecPkts == 0u)) {
    return 0u; 
  }

  for(auto it{0u}; it < nrOfRecPkts; ++it){
    packets[it] = new Packet(rxPackets[it]);
    //    packets[it] = rte_pktmbuf_mtod(rxPackets[it], ether_hdr*);  
  }

  return nrOfRecPkts;
}

void DpdkEngine::sendPackets(const Packet** pakcets, uint16_t pktCount) {
 //fill mbufs and send 

}

void DpdkEngine::freePackets(rte_ring* freeRing) {
  Packet* packets[TX_BURST_SIZE];
  auto nrOfPkts = rte_ring_dequeue_burst(freeRing, reinterpret_cast<void**>(packets), TX_BURST_SIZE, nullptr);
  //std::cout <<nrOfPkts << std::endl;
  if (nrOfPkts == 0u) {
    return; 
  }
  for(auto it{0u}; it < nrOfPkts; ++it) {
    rte_pktmbuf_free(packets[it]->getMBuf());
    //delete packets[it];
  }
}
