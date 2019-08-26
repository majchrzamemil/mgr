#ifndef CONFIG
#define CONFIG
#include <string>
#include <algorithm>

//constexpr char* DPDK_ARGS{"mgr -l 0,1 --socket-mem 512"};
//calculate this
constexpr int DPDK_ARGS_C {5};
constexpr uint16_t RT_LCORE{1u};
constexpr uint16_t PP_LCORE{2u};
constexpr uint16_t RING_SIZE{512u};

#endif
