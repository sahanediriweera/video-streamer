add_library(
  common_lib STATIC
  src/Config.cpp
  src/Connection.cpp
  src/Error.cpp
  src/Logger.cpp
  src/Packet.cpp
  src/Poller.cpp
  src/Socket.cpp
)

target_include_directories(common_lib PUBLIC include)