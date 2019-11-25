#ifndef CONFIG
#define CONFIG
#include <string>
#include <algorithm>

#include "Engine.hpp"

#include <rte_mempool.h>

enum class EngineType {
  SOCKET,
  DPDK
};

constexpr uint16_t RT_LCORE{1u};
constexpr uint16_t PP_LCORE{2u};
constexpr uint16_t RING_SIZE{2048u};
//constexpr EngineType ENGINE_TYPE{EngineType::DPDK};
constexpr EngineType ENGINE_TYPE{EngineType::SOCKET};

const EngineConfig config{
  .portId = 0u,
  .mBuffPoolSize = 4095u,
  .memPoolCashSize = 256u,
  .memPoolFlags = MEMPOOL_F_SP_PUT | MEMPOOL_F_SC_GET,
  .ipAddr = std::string{"192.168.1.1"},
  .tcpPort = 8080u,
  .txBurstSize = 512u,
  .rxBurstSize = 128u,
  .devName=std::string{"eno1"}
};
#endif
