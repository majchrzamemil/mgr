#include "../include/DpdkDevice.hpp"
#include "../include/Engine.hpp"

#include <memory>

class DpdkEngine : public Engine {
  public:
    bool initDpdk(int dpdkArgc, char** dpdkArgv);
    void startEngine();
  private:
    std::unique_ptr<DpdkDevice> device;
};
