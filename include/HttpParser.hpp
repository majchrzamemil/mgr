#ifndef HTTP_PARSER
#define HTTP_PARSER
#include <string>

#include<unordered_map>

#include"HttpRequest.hpp"
class HttpParser {
 public:
  static HttpRequest parseRequest(char* data);
 private:
  static std::unordered_map<std::string, RequestType> mRequestTypes;
};
#endif
