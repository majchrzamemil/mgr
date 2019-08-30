#ifndef CONFIG
#define CONFIG
#include <string>
#include <algorithm>

#include "Engine.hpp"

enum class EngineType {
  SOCKET,
  DPDK
};

constexpr uint16_t RT_LCORE{1u};
constexpr uint16_t PP_LCORE{2u};
constexpr uint16_t RING_SIZE{512u};
constexpr EngineType ENGINE_TYPE{EngineType::DPDK};

const EngineConfig config{
  .portId = 0u,
  .mBuffPoolSize = 2047u,
  .memPoolCashSize = 64u,
  .memPoolFlags = 0u,
  .ipAddr = std::string{"192.168.1.1"},
  .tcpPort = 8080u,
  .txBurstSize = 32u,
  .rxBurstSize = 64u,
  .devName=std::string{"enp0s25"}
};
#endif
