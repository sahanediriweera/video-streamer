add_library(
        server_lib STATIC
        src/Server.cpp
)

target_include_directories(server_lib PUBLIC include)
target_link_libraries(server_lib PRIVATE common_lib protocol_lib)

add_executable(
        server_exec
        src/main.cpp
)

target_link_libraries(server_exec PRIVATE server_lib)