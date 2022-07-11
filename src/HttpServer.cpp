//
// Created by OctAutumn on 2022/7/7.
//

#include "HttpServer.h"

long HttpServer::check_error(long ret, const char *msg)
{
    if (ret == -1)
    {
        perror(msg);
        exit(1);
    }
    return ret;
}

long HttpServer::check_error(long ret, const std::string &msg)
{
    if (ret == -1)
    {
        perror(msg.c_str());
        exit(1);
    }
    return ret;
}

void HttpServer::activate_nio(int fd)
{
    int flags = (int) check_error(fcntl(fd, F_GETFL), "> [E] fcntl GET error");
    flags |= O_NONBLOCK;
    check_error(fcntl(fd, F_SETFL, flags), "> [E] fcntl SET error");
}

void HttpServer::workFunc(HttpHandlerMap &httpHandlerMap, int &tmp_epoll_recv_fd, HttpRequest *request)
{
    HttpResponse *response;

    response = httpHandlerMap.handle(request);
    delete request;

    //向客户端发送数据
    std::string response_str = response->toString();
    delete response;
    const char *str = response_str.c_str();
    check_error(send(tmp_epoll_recv_fd, str, strlen(str), 0), "> [E] write error");
}

void HttpServer::start_server(int argc, char **argv)
{
    if (argc != 3)
    {
        printf("Usage: %s ip port\n", argv[0]);
        exit(1);
    }

    printf("+ [W] Server will be started on %s:%s\n", argv[1], argv[2]);

    char *ip = argv[1];
    char *port_endPtr;
    int port = (int) strtol(argv[2], &port_endPtr, 10);

    printf("- [I] Creating Serv_Socket...\n");
    //创建套接字
    int serv_sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    int clint_sock;

    printf("- [I] Binding IP and Port...\n");
    //将套接字和IP、端口绑定
    struct sockaddr_in serv_addr{};
    memset(&serv_addr, 0, sizeof(serv_addr));  //每个字节都用0填充
    serv_addr.sin_family = AF_INET;  //使用IPv4地址
    serv_addr.sin_port = htons(port);  //端口
    inet_pton(AF_INET, ip, &serv_addr.sin_addr);
    //开启端口复用
    bool on = true;
    setsockopt(serv_sock, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on));
    //绑定端口
    check_error(bind(serv_sock, (struct sockaddr *) &serv_addr, sizeof(serv_addr)),
                "> [E] bind error");

    //进入监听状态，等待用户发起请求
    check_error(listen(serv_sock, 20), "> [E] listen error");

    printf("- [I] Activating epoll...\n");
    //创建epoll实例
    struct epoll_event ev{}, events[MAX_EV_SIZE];   //通用Epoll实例
    int epoll_fd = (int) check_error(epoll_create(MAX_EV_SIZE),
                                     "> [E] epoll_create error");

    ev.data.fd = serv_sock;
    ev.events = EPOLLIN;
    check_error(epoll_ctl(epoll_fd, EPOLL_CTL_ADD, serv_sock, &ev),
                "> [E] epoll_ctl error");

    printf("- [I] Initializing ThreadPool...\n");
    ThreadPool threadPool;
    threadPool.init();
    printf("> [I] ThreadPool initialized with size: %d\n", threadPool.size());

    printf("\n");
    printf("===========>Server Activated<===========\n");
    printf("=====>Waiting for Client's request<=====\n");

    int n_counts;

    while (!server_shutdown)
    {
        n_counts = (int) check_error(epoll_wait(epoll_fd, events, MAX_EV_SIZE, -1),
                                     "> [E] epoll_wait error");
        if (n_counts == 0)
        {
            printf("Time out, no data.\n");
            continue;
        } else
        {
            int tmp_epoll_recv_fd;
            for (int i = 0; i < n_counts; i++)
            {
                tmp_epoll_recv_fd = events[i].data.fd;
                if (tmp_epoll_recv_fd == serv_sock)
                {//是连接请求
                    clint_sock = (int) check_error(accept(serv_sock, (struct sockaddr *) nullptr, nullptr),
                                                   "> [E] accept error");

                    activate_nio(clint_sock);   // 启用NIO模式

                    ev.events = EPOLLIN;
                    ev.data.fd = clint_sock;

                    check_error(epoll_ctl(epoll_fd, EPOLL_CTL_ADD, clint_sock, &ev),
                                "> [E] epoll_ctl ACC_NEW error");
                } else
                {//是已连接的数据
                    long n;
                    char buffer[BUFSIZ + 1];
                    n = check_error(recv(tmp_epoll_recv_fd, buffer, BUFSIZ, 0),
                                    "recv error at " + std::to_string(i) + " FD: " +
                                    std::to_string(tmp_epoll_recv_fd));
                    if (n == 0)
                    {
                        ev.events = EPOLLIN;
                        ev.data.fd = tmp_epoll_recv_fd;
                        check_error(epoll_ctl(epoll_fd, EPOLL_CTL_DEL, tmp_epoll_recv_fd, &ev),
                                    "epoll_ctl REMOVE_OLD error at " + std::to_string(i) + " FD: " +
                                    std::to_string(tmp_epoll_recv_fd));
                        close(tmp_epoll_recv_fd);
                    } else
                    {
                        buffer[n] = '\0';
                        //
                        HttpRequest *request;
                        try
                        {
                            request = new HttpRequest(buffer);    //HTTP请求解析
                        } catch (HttpException &e)
                        {
                            printf("An exception occurred when parsing an HttpRequest: %s\n", e.msg);
                            delete request;
                            HttpResponse response;

                            response.status_code = 400;
                            response.result_msg = "Bad Request";
                            response.parameters.insert({"Content-Type", "text/html;charset=utf-8"});
                            response.setContext("<h1>400 Bad Request</h1>");

                            //向客户端发送数据
                            std::string response_str = response.toString();
                            const char *str = response_str.c_str();
                            check_error(send(tmp_epoll_recv_fd, str, strlen(str), 0), "> [E] write error");
                            continue;
                        }
                        //异步处理接收到的客户端请求
                        threadPool.submit(workFunc, handlerMap, tmp_epoll_recv_fd, request);
                    }
                }
            }
        }
    }
}

void HttpServer::close_server()
{
    server_shutdown = true;
}
