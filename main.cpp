#include <model/HttpRequest.h>
#include <model/HttpResponse.h>
#include <server/HttpServer.h>

int main(int argc, char *argv[])
{
    HttpServer httpServer;

    httpServer.addHandler("/", "GET", [](const HttpRequest *request){
        auto *response = new HttpResponse{};

        response->parameters.insert({"Content-Type", "text/html;charset=utf-8"});
        response->setContext("Hello Client - / - GET");

        return response;
    });

    httpServer.start_server(argc, argv);

    return 0;
}