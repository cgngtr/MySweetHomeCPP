cmake_minimum_required(VERSION 3.16)
project(MySweetHome VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 98)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Include directories
include_directories(${CMAKE_SOURCE_DIR}/include)

# Add subdirectories for each module
add_subdirectory(src/Core)
add_subdirectory(src/Devices)
add_subdirectory(src/UI)
add_subdirectory(src/Logger)
add_subdirectory(src/SystemControl)

# Main executable
add_executable(MySweetHome src/main.cpp)

# Link all modules to main executable
target_link_libraries(MySweetHome
    PRIVATE
        Core
        Devices
        UI
        Logger
        SystemControl
)

# Optional: Enable testing
option(BUILD_TESTS "Build test executables" OFF)
if(BUILD_TESTS)
    enable_testing()
    add_subdirectory(tests)
endif()
