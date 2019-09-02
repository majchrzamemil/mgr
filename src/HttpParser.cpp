#include "../include/HttpParser.hpp"
#include <vector>
#include <sstream>
#include <algorithm>

#include <iostream>


std::unordered_map<std::string, RequestType> HttpParser::mRequestTypes{ {"GET", RequestType::GET} };

HttpRequest HttpParser::parseRequest(char* data) {
  constexpr char  delim = ' ';
  constexpr uint8_t requestTypeIndex{0u};
  constexpr uint8_t uriIndex{1u};

  HttpRequest request;
  std::vector<std::string> tokens;
  std::string requestString{data};
  std::stringstream requestStream{requestString};
  std::string token;

  while (std::getline(requestStream, token, delim)) {
    tokens.push_back(token);
  }
  if (auto reqType = mRequestTypes.find(tokens[requestTypeIndex]); reqType != mRequestTypes.end()) {
    request.setRequestType(reqType->second);
  } else {
    request.setRequestType(RequestType::NOT_REQUEST);
    return request;
  }
  request.setUri(tokens[uriIndex]);

  return request;
}
