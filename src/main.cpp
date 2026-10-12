#include "HttpServer.h"
#include <cwchar>

int main() {
    HttpServer server;
    if(server.init()){
        server.run();
        wprintf(L"Server init succes")
    } else {
        wprintf(L"Server init error");
    }
    return 0;
}