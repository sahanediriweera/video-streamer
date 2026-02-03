add_library(
        video_lib STATIC
        src/Capture.cpp
        src/Decoder.cpp
        src/Encoder.cpp
        src/Frame.cpp

        include/video/Capture.hpp
        include/video/Decoder.hpp
        include/video/Encoder.hpp
        include/video/Frame.hpp
)

target_include_directories(
        video_lib PUBLIC include
)