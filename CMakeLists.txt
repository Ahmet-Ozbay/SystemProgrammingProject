cmake_minimum_required(VERSION 3.10)
project(RCS VERSION 1.0)

# Set C++ standard
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# For Raspberry Pi, we need to link with specific libraries
if(${CMAKE_SYSTEM_NAME} MATCHES "Linux")
    find_package(Threads REQUIRED)
endif()

# Include directories
include_directories(include)

# Library source files
set(LIB_SOURCES
    src/GyroLib.cpp
    src/KeypadLib.cpp
    src/DigSensorLib.cpp
    src/RelayLib.cpp
    src/SocketConLib.cpp
)

# Create a static library with the common code
add_library(rcs_lib STATIC ${LIB_SOURCES})

# Define executables for each node
add_executable(GyroSensorNode GyroSensorNode.cpp)
add_executable(DigitalIONode DigitalIONode.cpp)
add_executable(ServerNode ServerNode.cpp)
add_executable(ClientNode ClientNode.cpp)

# Link libraries to executables
target_link_libraries(GyroSensorNode rcs_lib)
target_link_libraries(DigitalIONode rcs_lib)
target_link_libraries(ServerNode rcs_lib)
target_link_libraries(ClientNode rcs_lib)

# For Linux/Raspberry Pi, we need to link against additional libraries
if(${CMAKE_SYSTEM_NAME} MATCHES "Linux")
    # Link with pthread library
    target_link_libraries(GyroSensorNode ${CMAKE_THREAD_LIBS_INIT})
    target_link_libraries(DigitalIONode ${CMAKE_THREAD_LIBS_INIT})
    target_link_libraries(ServerNode ${CMAKE_THREAD_LIBS_INIT})
    target_link_libraries(ClientNode ${CMAKE_THREAD_LIBS_INIT})
    
    # If we need to link with GPIO libraries (for Raspberry Pi)
    if(EXISTS "/usr/include/wiringPi.h")
        target_link_libraries(GyroSensorNode wiringPi)
        target_link_libraries(DigitalIONode wiringPi)
        target_link_libraries(ServerNode wiringPi)
    endif()
endif()

# Installation rules
install(TARGETS GyroSensorNode DigitalIONode ServerNode ClientNode
    RUNTIME DESTINATION bin
)

# Print configuration information
message(STATUS "Build type: ${CMAKE_BUILD_TYPE}")
message(STATUS "C++ compiler: ${CMAKE_CXX_COMPILER_ID} ${CMAKE_CXX_COMPILER_VERSION}")