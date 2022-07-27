#include <string>

#include <model/HttpRequest.h>
#include <model/HttpResponse.h>
#include <server/HttpServer.h>

int main(int argc, char *argv[])
{
    HttpServer httpServer;

    httpServer.addHandler("/", "GET", [](const HttpRequest *request){
        auto *response = new HttpResponse{};
        std::string context = "";

        for ( auto& pair: request->URL_parameters ) {
            context = context + pair.first + ":";
            for( auto& param: pair.second ) {
                context = context + param + ",";
            }
            context += "\n";
        }

        response->parameters.insert({"Content-Type", "text/html;charset=utf-8"});
        response->setContext(context);

        return response;
    });

    httpServer.start_server(argc, argv);

    return 0;
}