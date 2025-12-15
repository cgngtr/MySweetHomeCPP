enable_testing()

# Test executable for devices
add_executable(test_devices test_devices.cpp)
target_link_libraries(test_devices
    PRIVATE
        Core
        Devices
        Logger
)
target_include_directories(test_devices
    PRIVATE
        ${CMAKE_SOURCE_DIR}/include
        ${CMAKE_SOURCE_DIR}/src/Core
        ${CMAKE_SOURCE_DIR}/src/Devices
        ${CMAKE_SOURCE_DIR}/src/Logger
)
add_test(NAME DeviceTests COMMAND test_devices)

# Test executable for menu
add_executable(test_menu test_menu.cpp)
target_link_libraries(test_menu
    PRIVATE
        Core
        Devices
        UI
        Logger
        SystemControl
)
target_include_directories(test_menu
    PRIVATE
        ${CMAKE_SOURCE_DIR}/include
        ${CMAKE_SOURCE_DIR}/src/Core
        ${CMAKE_SOURCE_DIR}/src/Devices
        ${CMAKE_SOURCE_DIR}/src/UI
        ${CMAKE_SOURCE_DIR}/src/Logger
        ${CMAKE_SOURCE_DIR}/src/SystemControl
)
add_test(NAME MenuTests COMMAND test_menu)
