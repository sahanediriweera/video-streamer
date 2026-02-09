add_library(
        server STATIC
        src/Server.cpp
)

target_include_directories(server PUBLIC include)