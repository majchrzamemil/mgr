#include "../include/HttpParser.hpp"
#include <vector>
#include <sstream>
#include <algorithm>


std::unordered_map<std::string, RequestType> HttpParser::mRequestTypes{ {"GET", RequestType::GET} };

std::unordered_map<ResponseType, std::string> HttpParser::mResponseTypes{ {ResponseType::OK, "OK"}, {ResponseType::NOT_FOUND, "NOT_FOUND"}};

void HttpParser::parseRequest(char* data, HttpRequest* request) {
  constexpr char  delim = ' ';
  constexpr uint8_t requestTypeIndex{0u};
  constexpr uint8_t uriIndex{1u};
  constexpr uint8_t versionIndex{2u};

  std::vector<std::string> tokens;
  std::string requestString{data};
  std::stringstream requestStream{requestString};
  std::string token;

  while (std::getline(requestStream, token, delim)) {
    tokens.push_back(token);
  }

  if (tokens.size() < 3u) {
    request->setRequestType(RequestType::NOT_REQUEST);
    return;
  }
  if (auto reqType = mRequestTypes.find(tokens[requestTypeIndex]); reqType != mRequestTypes.end()) {
    request -> setRequestType(reqType->second);
  } else {
    request->setRequestType(RequestType::NOT_REQUEST);
    return;
  }
  request->setUri(tokens[uriIndex]);
  request->setRequestVersion(tokens[versionIndex]);

}

std::string HttpParser::parseResponse(HttpResponse* response) {
  std::string responseString {};
  constexpr char delim = ' ';
  const std::string endLine = "\r\n ";
  responseString += response->getResponseVersion() + delim;
  if (auto respType = mResponseTypes.find(response->getResponseType());
      respType != mResponseTypes.end()) {
    responseString += respType->second + delim;
  } else {
    return "";
  }
  responseString += std::to_string(response->getResponseType()) + delim;
  responseString += endLine;
  responseString += response->getPayload();
  return responseString;
}
