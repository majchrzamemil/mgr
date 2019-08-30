#include "../include/Engine.hpp"

bool Engine::init(int dpdkArgc, char** dpdkArgv, const EngineConfig& config) {
  if (rte_eal_init(dpdkArgc, dpdkArgv) < 0) {
    return false;
  }
}
