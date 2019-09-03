#include "../include/HttpParser.hpp"
#include <vector>
#include <sstream>
#include <algorithm>

#include <iostream>


std::unordered_map<std::string, RequestType> HttpParser::mRequestTypes{ {"GET", RequestType::GET} };

std::unordered_map<ResponseType, std::string> HttpParser::mResponseTypes{ {ResponseType::OK, "OK"} };

void HttpParser::parseRequest(char* data, HttpRequest* request) {
  constexpr char  delim = ' ';
  constexpr uint8_t requestTypeIndex{0u};
  constexpr uint8_t uriIndex{1u};

  std::vector<std::string> tokens;
  std::string requestString{data};
  std::stringstream requestStream{requestString};
  std::string token;

  while (std::getline(requestStream, token, delim)) {
    tokens.push_back(token);
  }
  if (auto reqType = mRequestTypes.find(tokens[requestTypeIndex]); reqType != mRequestTypes.end()) {
    request -> setRequestType(reqType->second);
  } else {
    request->setRequestType(RequestType::NOT_REQUEST);
    return;
  }
  //add validation
  request->setUri(tokens[uriIndex]);
  //also add validation
  request->setRequestVersion(tokens[2]);

}

std::string HttpParser::parseResponse(HttpResponse* response) {
  std::string responseString {};
  constexpr char delim = ' ';
  responseString += response->getResponseVersion() + delim;
  if (auto respType = mResponseTypes.find(response->getResponseType()); respType != mResponseTypes.end()) {
    responseString += respType->second + delim;
  } else {
    return "";
  }
  responseString += std::to_string(response->getResponseType()) + delim;
  responseString += "\r\n ";
  responseString += response->getPayload();
  return responseString;
}
