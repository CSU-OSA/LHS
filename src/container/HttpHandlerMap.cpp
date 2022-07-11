//
// Created by OctAutumn on 2022/7/7.
//

#include "HttpHandlerMap.h"

void HttpHandlerMap::addHandler(const std::string &url_path, const std::string &method,
                                const std::function<HttpResponse *(const HttpRequest *)> &function)
{
    std::string method_upper = method;
    transform(method_upper.begin(), method_upper.end(), method_upper.begin(),
              [](unsigned char c) { return toupper(c); });

    printf("- [I] HandlerMap > Added a handler > K: %s\n", (url_path + "::" + method_upper).c_str());
    handler_map.insert({url_path + "::" + method_upper, function});
}

HttpResponse *HttpHandlerMap::handle(const HttpRequest *request)
{
    HttpResponse *response;
    try
    {
        std::string method_upper = request->method;
        transform(method_upper.begin(), method_upper.end(), method_upper.begin(),
                  [](unsigned char c) { return toupper(c); });

        std::string K = request->URL_path + "::" + method_upper;
        printf("Received Request > K: %s\n", K.c_str());

        std::function<HttpResponse *(const HttpRequest *)> handler = handler_map.at(request->URL_path + "::" + method_upper);
        response = handler(request);
    } catch (std::out_of_range &e)
    {
        response = new HttpResponse();
        response->status_code = 404;
        response->result_msg = "Not Found";
        response->parameters.insert({"Content-Type", "text/html;charset=utf-8"});
        response->setContext("<h1>404 Not Found</h1>");
        return response;
    }
    return response;
}
