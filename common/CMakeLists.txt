add_library(
  common_lib STATIC
  src/Socket.cpp src/Poller.cpp
        src/Connection.cpp
)

add_subdirectory(common)

target_link_libraries(server PRIVATE common)

target_include_directories(common_lib PUBLIC include)