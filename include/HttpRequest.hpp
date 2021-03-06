#ifndef HTTP
#define HTTP
#include <string>
#include <cstdint>

#include "Packet.hpp"

enum RequestType {
  GET,
  POST,
  DELETE,
  PUT,
  NOT_REQUEST
};

class HttpRequest {
 public:
  HttpRequest(Packet* packet): mPacket{packet} {};
  void setRequestType(const RequestType requestType) {
    mRequestType = requestType;
  }
  RequestType getRequestType() const {
    return mRequestType;
  }
  void setUri(const std::string& uri) {
    mUri = uri;
  }
  std::string getUri() const {
    return mUri;
  }
  std::string getRequestVersion() const {
    return mRequestVersion;
  }
  void setRequestVersion(const std::string& requestVersion) {
    mRequestVersion = requestVersion;
  }
  Packet* getPacket() const {
    return mPacket;
  }
  ~HttpRequest() {}
 private:
  Packet* mPacket;
  RequestType mRequestType;
  std::string mRequestVersion;
  std::string mUri;
  std::string mPayload;
};
#endif
