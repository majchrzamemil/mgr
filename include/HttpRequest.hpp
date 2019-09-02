#ifndef HTTP
#define HTTP
#include <string>
#include <cstdint>
enum class RequestType : uint8_t {
  NOT_REQUEST,
  GET,
  POST,
  DELETE,
  PUT
};

class HttpRequest {
  public:
    HttpRequest() = default;
    void setRequestType(const RequestType requestType) { mRequestType = requestType; }
    RequestType getRequestType() const { return mRequestType;} 
    void setUri(const std::string& uri) { mUri=uri;}
    std::string getUri() const { return mUri;}
  private:
    RequestType mRequestType;
    std::string mRequestVersion;
    std::string mUri;
    //for now
    std::string mPayload;
};
#endif
