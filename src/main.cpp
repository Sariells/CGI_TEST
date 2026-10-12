#include "HttpServer.h"
#include <cwchar>

int main() {
    HttpServer server;
    if(server.init()){
        server.run();
    } else {
        wprintf(L"Server init error");
    }
    return 0;
}