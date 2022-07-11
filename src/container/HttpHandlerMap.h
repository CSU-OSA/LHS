//
// Created by OctAutumn on 2022/7/7.
//

#ifndef LHS_HTTPHANDLERMAP_H
#define LHS_HTTPHANDLERMAP_H

#include <unordered_map>
#include <string>
#include <functional>
#include <stdexcept>
#include <algorithm>

#include "model/HttpResponse.h"
#include "model/HttpRequest.h"

class HttpHandlerMap
{
private:
    std::unordered_map<std::string, std::function<HttpResponse*(const HttpRequest*)>> handler_map; // 处理函数
public:
    HttpHandlerMap() = default;

    void addHandler(const std::string &url_path, const std::string &method, const std::function<HttpResponse*(const HttpRequest*)>& function);

    HttpResponse *handle(const HttpRequest *request);
};


#endif //LHS_HTTPHANDLERMAP_H
