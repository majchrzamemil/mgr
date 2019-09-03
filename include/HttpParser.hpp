#ifndef HTTP_PARSER
#define HTTP_PARSER
#include <string>

#include<unordered_map>

#include"HttpRequest.hpp"
#include"HttpResponse.hpp"

class HttpParser {
 public:
  static void parseRequest(char* data, HttpRequest* request);
  static std::string parseResponse(HttpResponse* response);
 private:
  static std::unordered_map<std::string, RequestType> mRequestTypes;
  static std::unordered_map<ResponseType, std::string> mResponseTypes;
};
#endif
