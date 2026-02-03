add_library(
  common_lib STATIC
  src/Socket.cpp src/Poller.cpp
        src/Connection.cpp
)

target_include_directories(common_lib PUBLIC include)