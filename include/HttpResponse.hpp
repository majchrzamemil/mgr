#ifndef HTTP_RESPONSE
#define HTTP_RESPONSE
#include <string>
#include <cstdint>

enum ResponseType {
  OK = 200,
  NOT_FOUND = 404
};

class HttpResponse {
 public:
  HttpResponse() = default;
  void setResponseType(const ResponseType responseType) {
    mResponseType = responseType;
  }
  ResponseType getResponseType() const {
    return mResponseType;
  }
  std::string getResponseVersion() const {
    return mResponseVersion;
  }
  void setResponseVersion(const std::string& responseVersion) {
    mResponseVersion = responseVersion;
  }
  void setPayload(const std::string& payload) {
    mPayload = payload;
  }
  std::string getPayload() const {
    return mPayload;
  }
 private:
  ResponseType mResponseType;
  std::string mResponseVersion;
  std::string mPayload;
  std::string mContentType;
};

#endif
