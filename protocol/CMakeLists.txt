add_library(
        protocol_lib STATIC
        src/Message.cpp
        src/MessageType.cpp
        src/Serializer.cpp

        includes/protocol/Message.hpp
        includes/protocol/MessageType.hpp
        includes/protocol/Serializer.hpp
)

target_include_directories(common_lib PUBLIC includes)