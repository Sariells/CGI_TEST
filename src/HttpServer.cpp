//
// Created by user on 12.10.2026.
//
#include <cwchar>
#include <string>
#include "HttpServer.h"

HttpServer::HttpServer() = default;
HttpServer::~HttpServer() {
    unload();
}

void HttpServer::run() {

    std::string html = "<h1> HELLO FROM ME!</h1>";
    std::string headers =
            "HTTP/1.1 200 OK\r\n"
            "content-Type: text/html;charset=utf8\r\n"
            "Content-Length: " +std::to_string(html.size()) + "\r\n"
            "Connection: close\r\n"
            "\r\n";
    std::string responce = headers + html;

    while(true){
        SOCKET Clientsocket = accept(Listensocket,nullptr,nullptr);
        if(Clientsocket == INVALID_SOCKET){
            wprintf(L"accept() function failed with error: %d\n",WSAGetLastError());
            return;
        }

        int rBytes= recv(Clientsocket,buffer,sizeof(buffer),0);
        if(rBytes == SOCKET_ERROR){
            wprintf(L"recv() function failed with error: %d\n",WSAGetLastError());
            closesocket(Clientsocket);
            continue;
        }
        if(rBytes == 0 ){
            wprintf(L"Client Disconnected\n");
            closesocket(Clientsocket);
            continue;
        }else{
            wprintf(L"Received %d bytes\n", rBytes);
            wprintf(L"%.*hs\n", rBytes, buffer);
        }
        int totalSent = 0;
        int responceSize = static_cast<int>(responce.size());
        while(totalSent < responceSize){
            int sent = send(Clientsocket,
                            responce.data() + totalSent,
                            responceSize - totalSent,
                            0);
            if(sent == SOCKET_ERROR || sent == 0){
                wprintf(L"send() failed or conection closed\n");
                break;
            }
            totalSent += sent;
        }

        closesocket(Clientsocket);
    }
}
void HttpServer::unload() {
    if(Listensocket != INVALID_SOCKET){
        closesocket(Listensocket);
    }
    if(winsocksucces){
        WSACleanup();
        winsocksucces = false;
    }
}
bool HttpServer::init() {
    iResult = WSAStartup(MAKEWORD(2, 2), &data);

    if(iResult != NO_ERROR){
        wprintf(L"Error at WSAStartup()\n");
        return false;
    }
    winsocksucces = true;

    Listensocket = socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);

    if(Listensocket == INVALID_SOCKET){
        wprintf(L"socket function failed with error: %u\n",WSAGetLastError());
        return false;
    }

    service.sin_family = AF_INET;
    service.sin_port = htons(8080);

    iResult = inet_pton(AF_INET,
              "127.0.0.1",
              &service.sin_addr);
    if(iResult != 1){
        wprintf(L"inet_pton function failed with error %u\n",WSAGetLastError());
        return false;
    }
    iResult = bind(Listensocket, (SOCKADDR *) &service, sizeof(service));

    if (iResult == SOCKET_ERROR) {
        wprintf(L"bind failed with error %u\n", WSAGetLastError());
        closesocket(Listensocket);
        WSACleanup();
        return false;
    }
    if(listen(Listensocket,SOMAXCONN) == SOCKET_ERROR) {
        wprintf(L"Listen() function failed with error: %d\n", WSAGetLastError());
        closesocket(Listensocket);
        WSACleanup();
        return false;
    }
    return true;
}