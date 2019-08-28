#include "../include/SocketDevice.hpp"

#include <arpa/inet.h>
#include <string.h>
#include <sys/socket.h>

bool SocketDevice::startDevice() {
  //change later for TCP
  mSocketDesc = socket(AF_INET, SOCK_RAW, IPPROTO_UDP);
  if (mSocketDesc == -1) {
    perror("Failed to create socket");
    return false;
  }

  if (setsockopt(mSocketDesc, SOL_SOCKET, SO_BINDTODEVICE, mDevName.c_str(), strlen(mDevName.c_str())) == -1) {
    perror("Failed to set device");
    return false;
  }
  return false;

}
bool SocketDevice::setIpAddr(std::string ipStr) {
  return (inet_pton(AF_INET, ipStr.c_str(), reinterpret_cast<void*>(&mIpAddr)) == 1) ? true : false;
}

