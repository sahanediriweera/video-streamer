add_library(
        protocol_lib STATIC
        src/Message.cpp
        src/MessageType.cpp
        src/Serializer.cpp
)

target_include_directories(protocol_lib PUBLIC includes)