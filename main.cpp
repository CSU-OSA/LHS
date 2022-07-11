#include "src/model/HttpRequest.h"
#include "src/model/HttpResponse.h"
#include "HttpServer.h"

int main(int argc, char *argv[])
{
    HttpServer httpServer;

    httpServer.addHandler("/", "GET", [](const HttpRequest *request){
        auto *response = new HttpResponse{};

        printf(" > Received a \"/\" Request\n");
        if (!request->URL_parameters.empty())
        {
            printf(" >- This request has URL-parameters:\n");
            for (auto &it:request->URL_parameters)
                printf(" >-- %s:%s\n", it.first.c_str(), it.second.c_str());
        }

        if (!request->parameters.empty())
        {
            printf(" >- This request has Request-parameters:\n");
            for (auto &it:request->parameters)
                printf(" >-- %s:%s\n", it.first.c_str(), it.second.c_str());
        }

        response->parameters.insert({"Content-Type", "text/html;charset=utf-8"});
        response->setContext("Hello Client - / - GET");

        return response;
    });

    httpServer.addHandler("/", "POST", [](const HttpRequest *request){
        auto *response = new HttpResponse{};

        response->parameters.insert({"Content-Type", "text/html;charset=utf-8"});
        response->setContext("Hello Client - / - POST");

        return response;
    });

    httpServer.addHandler("/test", "GET", [](const HttpRequest *request){
        auto *response = new HttpResponse{};

        response->parameters.insert({"Content-Type", "text/html;charset=utf-8"});
        response->setContext("Hello Client - /test - GET");

        return response;
    });

    httpServer.start_server(argc, argv);

    return 0;
}