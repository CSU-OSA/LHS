//
// Created by OctAutumn on 2022/7/4.
//

#ifndef LHS_HTTPREQUEST_H
#define LHS_HTTPREQUEST_H

#include <string>
#include <cstring>
#include <unordered_map>
#include "HttpException.h"
#include <container/UrlParams.h>

class HttpRequest
{
public:

    std::string method;
    std::string URL_path;
    std::unordered_map<std::string, UrlParams> URL_parameters;
    std::string version;
    std::unordered_map<std::string, std::string> parameters;
    std::string context;

    explicit HttpRequest(const char *httpMsg_Str);

    ~HttpRequest() = default;

private:
    int parseHttpHead_requestLine(const char *a_line);

    int parseHttpHead_requestParams(const char *a_line);
};


#endif //LHS_HTTPREQUEST_H
