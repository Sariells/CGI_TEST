#include <cwchar>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <string>

int main() {
    WSADATA data{};

    int iResult = 0;

    SOCKET Listensocket = INVALID_SOCKET;

    sockaddr_in service{};

    char buffer[4096]{};

    std::string html = "<h1> HELLO FROM ME!</h1>";
    std::string headers =
            "HTTP/1.1 200 OK\r\n"
            "content-Type: text/html;charset=utf8\r\n"
            "Content-Length: " +std::to_string(html.size()) + "\r\n"
            "Connection: close\r\n"
            "\r\n";
    std::string responce = headers + html;
    iResult = WSAStartup(MAKEWORD(2, 2), &data);

    if(iResult != NO_ERROR){
        wprintf(L"Error at WSAStartup()\n");
        return 1;
    }

    Listensocket = socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);

    if(Listensocket == INVALID_SOCKET){
        wprintf(L"socket function failed with error: %u\n",WSAGetLastError());
        WSACleanup();
        return 1;
    }

    service.sin_family = AF_INET;
    service.sin_port = htons(8080);

    inet_pton(AF_INET,
              "127.0.0.1",
              &service.sin_addr);


    iResult = bind(Listensocket, (SOCKADDR *) &service, sizeof(service));

    if (iResult == SOCKET_ERROR) {
        wprintf(L"bind failed with error %u\n", WSAGetLastError());
        closesocket(Listensocket);
        WSACleanup();
        return 1;
    } else {
        wprintf(L"bind succes\n");
    }


    if( listen(Listensocket,SOMAXCONN) == SOCKET_ERROR) {
        wprintf(L"Listen() function failed with error: %d\n", WSAGetLastError());
        closesocket(Listensocket);
        WSACleanup();
        return 1;
    }

    SOCKET Clientsocket = accept(Listensocket,nullptr,nullptr);
    if(Clientsocket == INVALID_SOCKET){
        wprintf(L"accept() function failed with error: %d\n",WSAGetLastError());
        closesocket(Listensocket);
        WSACleanup();
        return 1;
    }

    int rBytes= recv(Clientsocket,buffer,sizeof(buffer),0);
    if(rBytes == SOCKET_ERROR){
        wprintf(L"recv() function failed with error: %d\n",WSAGetLastError());
    }
    else if(rBytes == 0 ){
        wprintf(L"Client Disconnected\n");
    }else{
        wprintf(L"Received %d bytes\n", rBytes);
        wprintf(L"%.*hs\n", rBytes, buffer);
    }

    send(Clientsocket,responce.data(),responce.size(),0);

    closesocket(Clientsocket);
    closesocket(Listensocket);
    WSACleanup();
    return 0;
}