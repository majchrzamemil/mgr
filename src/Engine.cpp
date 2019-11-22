#include "../include/Engine.hpp"
#include <iostream>

bool Engine::init(int dpdkArgc, char** dpdkArgv, const EngineConfig& config) {
  if (auto rt = rte_eal_init(dpdkArgc, dpdkArgv) < 0) {
    std::cout << rt; 
    return false;
  }
  return true;
}
