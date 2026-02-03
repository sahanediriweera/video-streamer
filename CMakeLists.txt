cmake_minimum_required(VERSION 3.15)
project(VideoStreamProject VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY  "${CMAKE_BINARY_DIR}/bin")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib")

if(APPLE)
    set(CMAKE_OSX_DEPLOYMENT_TARGET  "15.0" CACHE STRING "Minimum macOS version") #I set it to my mac version

    list(APPEND CMAKE_PREFIX_PATH "/opt/homebrew" "/usr/local")

endif()

if(MSVC)
    add_compile_options(/W4 /Wx)
else ()
    add_compile_options(-Wall -Wextra -Wpedantic -Werror)
endif ()

set(CMAKE_POSITION_INDEPENDENT_CODE ON)

add_subdirectory(common)
add_subdirectory(protocol)
add_subdirectory(video)
add_subdirectory(server)
add_subdirectory(client)
add_subdirectory(tests)
