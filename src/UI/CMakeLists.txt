add_library(UI
    Menu.cpp
    ConsoleUtils.cpp
    MenuCommands.cpp
    CommandInvoker.cpp
)

target_include_directories(UI
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_SOURCE_DIR}/include
        ${CMAKE_SOURCE_DIR}/src/Core
        ${CMAKE_SOURCE_DIR}/src/Devices
        ${CMAKE_SOURCE_DIR}/src/SystemControl
        ${CMAKE_SOURCE_DIR}/src/Logger
)

target_link_libraries(UI
    PUBLIC
        Core
        Devices
        Logger
)
