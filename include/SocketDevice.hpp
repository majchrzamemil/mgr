#ifndef SOCKET_DEVICE
#define SOCKET_DEVICE

#include <sys/socket.h>
#include <netinet/ip.h>

#include <string>

class SocketDevice {
 public:
  SocketDevice(const std::string& devName): mIpAddr{0u}, mSocketDesc{-1}, mDevName{devName} {}
  bool startDevice();

  bool setIpAddr(std::string ipStr);
  in_addr getIpAddr() {
    return mIpAddr;
  }
  int getSocketDesc() const {
    return mSocketDesc;
  }
 private:
  in_addr mIpAddr;
  int mSocketDesc;
  const std::string mDevName;
};
#endif
