//
// Created by OctAutumn on 2022/7/4.
//

#include "HttpResponse.h"

#include <utility>

HttpResponse::HttpResponse(std::string version, const int &status_code, std::string result_msg,
                           std::unordered_map<std::string, std::string> parameters, const std::string &context)
        : version(std::move(version)), status_code(status_code), result_msg(std::move(result_msg)),
          parameters(std::move(parameters)),
          context(context)
{
    char tAll[255];
    strftime(tAll, sizeof(tAll), "%a, %d %b %Y %H:%M:%S GMT",
             std::gmtime(new std::time_t(std::time(nullptr))));
    std::string HTTP_TIME(tAll);
    this->parameters.insert({"Date", HTTP_TIME});

    this->parameters.insert({"Connection", "close"});
    this->parameters.insert({"Content-Length", std::to_string(context.size())});
}

HttpResponse::HttpResponse()
{
    char tAll[255];
    strftime(tAll, sizeof(tAll), "%a, %d %b %Y %H:%M:%S GMT",
             std::gmtime(new std::time_t(std::time(nullptr))));
    std::string HTTP_TIME(tAll);
    this->parameters.insert({"Date", HTTP_TIME});

    this->parameters.insert({"Connection", "close"});
}

std::string HttpResponse::toString()
{
    std::string response = (version + ' ' + std::to_string(status_code) + ' ' + result_msg + "\r\n");   //组装响应头部

    //组装响应参数部分
    for (auto &item: parameters)
        response += (item.first + ": " + item.second + "\r\n");
    response += "\r\n";

    //组装响应正文
    response += context;

    if (!context.empty())
        response += "\r\n";

    return response;
}

void HttpResponse::setContext(std::string context_arg)
{
    this->context = std::move(context_arg);
    this->parameters.insert({"Content-Length", std::to_string(context.size())});
}

