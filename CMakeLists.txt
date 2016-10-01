cmake_minimum_required(VERSION 3.6)

# Enable debug symbols by default
# must be done before project() statement
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug CACHE STRING "Choose the type of build (Debug or Release)" FORCE)
endif()
# (you can also set it on the command line: -D CMAKE_BUILD_TYPE=Release)

if(MINGW)
    message(WARNING "Using MinGW compiler.")
    set(SFML_ROOT           "D:/Dev/Lib/SFML-2.4.0-windows-gcc-6.1.0-mingw-64-bit/SFML-2.4.0")
    set(CMAKE_CXX_FLAGS     "${CMAKE_CXX_FLAGS} -std=c++11")
endif()

if (UNIX)
    message(WARNING "Detected UNIX.")
    # use the pthread flag when compiling for linux
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -lpthread -pthread")
endif()

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "bin/")

project(DiveLight)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "bin/")

include_directories("${PROJECT_BINARY_DIR}")
include_directories("${PROJECT_BINARY_DIR}/include")


# Add KissFFT includes and sources
include_directories("${CMAKE_SOURCE_DIR}/external/kissfft")
include_directories("${CMAKE_SOURCE_DIR}/external/kissfft/tools")
set(KISSFFT_SOURCE
        ${CMAKE_SOURCE_DIR}/external/kissfft/kiss_fft.c
        ${CMAKE_SOURCE_DIR}/external/kissfft/tools/kiss_fftr.c)


# Add EasyWSClient
include_directories("${CMAKE_SOURCE_DIR}/external/easywsclient")
set(WSCLIENT_SOURCE
        ${CMAKE_SOURCE_DIR}/external/easywsclient/easywsclient.cpp
        ${CMAKE_SOURCE_DIR}/external/easywsclient/easywsclient.hpp)


set(EXECUTABLE_NAME "DiveLight")
set(SOURCE_FILES src/main.cpp include/config.hpp src/client.cpp include/client.hpp src/stream.cpp include/stream.hpp src/fft.cpp include/fft.hpp include/utils.hpp src/renderer.cpp include/renderer.hpp src/columnrenderer.cpp include/columnrenderer.hpp src/visualizer.cpp include/visualizer.hpp src/input.cpp include/input.hpp src/ledclient.cpp include/ledclient.hpp src/menu.cpp include/menu.hpp)
# add sources from directory and from external directories
add_executable(${EXECUTABLE_NAME} ${SOURCE_FILES} ${KISSFFT_SOURCE} ${WSCLIENT_SOURCE})


# Link win sockets when on windows
# Used by easywsclient
if (WIN32)
    target_link_libraries(${EXECUTABLE_NAME} wsock32 ws2_32)
endif()


set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake_modules" ${CMAKE_MODULE_PATH})
#Find any version 2.X of SFML
#See the FindSFML.cmake file for additional details and instructions
find_package(SFML 2 REQUIRED system window graphics network audio)
if(SFML_FOUND)
    include_directories(${SFML_INCLUDE_DIR})
    target_link_libraries(${EXECUTABLE_NAME} ${SFML_LIBRARIES})
endif()

# Install target
install(TARGETS ${EXECUTABLE_NAME} DESTINATION ${DiveLight_BINARY_DIR})
