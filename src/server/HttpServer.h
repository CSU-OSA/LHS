//
// Created by OctAutumn on 2022/7/7.
//

#ifndef LHS_HTTPSERVER_H
#define LHS_HTTPSERVER_H

#include <cstdio>
#include <cstring>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <cstdlib>
#include <thread>
#include <sys/epoll.h>
#include <fcntl.h>

#include "model/HttpRequest.h"
#include "model/HttpResponse.h"
#include "threadPool/ThreadPool.h"
#include "container/HttpHandlerMap.h"

#define MAX_EV_SIZE 1024

class HttpServer
{
private:
    HttpHandlerMap handlerMap{};

    bool server_shutdown = false;

    /**
    * 错误检查
    * @param ret 返回值
    * @param msg 返回错误信息
    * @return 正常返回值
    */
    static long check_error(long ret, const char *msg);

    static long check_error(long ret, const std::string &msg);

    static void activate_nio(int fd);

    static void workFunc(HttpHandlerMap &httpHandlerMap, int &tmp_epoll_recv_fd, HttpRequest *request);

public:
    HttpServer(): server_shutdown(false){};

    void addHandler(const std::string &url_path, const std::string &method,
                    const std::function<HttpResponse *(const HttpRequest *)> &func)
    {
        handlerMap.addHandler(url_path, method, func);
    }

    void start_server(int argc, char *argv[]);

    void close_server();
};


#endif //LHS_HTTPSERVER_H
