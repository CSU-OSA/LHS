//
// Created by OctAutumn on 2022/7/4.
//

#ifndef LHS_HTTPRESPONSE_H
#define LHS_HTTPRESPONSE_H

#include <string>
#include <cstring>
#include <unordered_map>
#include <ctime>
#include <utility>

class HttpResponse
{
public:
    std::string version = "HTTP/1.1";
    int status_code = 200;
    std::string result_msg = "OK";
    std::unordered_map<std::string, std::string> parameters;
    std::string context;

    explicit HttpResponse(std::string version, const int &status_code, std::string result_msg,
                          std::unordered_map<std::string, std::string> parameters, const std::string &context);

    explicit HttpResponse();

    ~HttpResponse() = default;

    std::string toString();

    void setContext(std::string context);
};


#endif //LHS_HTTPRESPONSE_H
