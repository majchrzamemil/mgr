#include "../include/DpdkEngine.hpp"

#include <rte_ethdev.h>
#include <rte_config.h>
#include <rte_bus_pci.h>


#include <iostream>
bool DpdkEngine::initDpdk(int dpdkArgc, char** dpdkArgv) {
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
