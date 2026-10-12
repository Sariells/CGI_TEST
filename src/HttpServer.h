//
// Created by user on 12.10.2026.
//

#ifndef CGI_TEST_HTTPSERVER_H
#define CGI_TEST_HTTPSERVER_H

#include <winsock2.h>
#include <ws2tcpip.h>

class HttpServer {
public:
    HttpServer();
    ~HttpServer();

    bool init();
    void run();
private:

    void unload();

    WSADATA data{};
    sockaddr_in service{};
    SOCKET Listensocket = INVALID_SOCKET;
    char buffer[4096]{};
    int iResult = 0;
    bool winsocksucces = false;
};


#endif //CGI_TEST_HTTPSERVER_H
