cmake_minimum_required(VERSION 4.0)
project(CGI_TEST)

set(CMAKE_CXX_STANDARD 26)
set(CMAKE_CXX_STANDARD_REQUIRED)
set(CMAKE_CXX_EXTENSIONS OFF)

add_executable(CGI_TEST src/main.cpp
        src/httpServer.cpp
        src/httpServer.h)
target_include_directories(CGI_TEST PRIVATE src)
if(MSVC)
    target_compile_options(CGI_TEST PRIVATE
            /std:c++latest
            /W4
            /permissive-
            /EHsc
    )
endif()
target_link_libraries(CGI_TEST PRIVATE ws2_32)
