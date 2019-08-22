#include "../include/DpdkDevice.hpp"
#include "../include/Engine.hpp"

#include <memory>

class DpdkEngine : public Engine {
  public:
    bool initDpdk(int dpdkArgc, char** dpdkArgv);
    void startEngine();
    uint8_t receivePackets(Http**);
    void sendPackets(const Http**, uint8_t pktCount);
  private:
    std::unique_ptr<DpdkDevice> device;
};
