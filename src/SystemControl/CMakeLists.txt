add_library(SystemControl
    SmartHome.cpp
    StateManager.cpp
    StateMemento.cpp
    ModeManager.cpp
    SecurityManager.cpp
    ISystemState.cpp
    SecurityColleague.cpp
)

target_include_directories(SystemControl
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_SOURCE_DIR}/include
        ${CMAKE_SOURCE_DIR}/src/Core
        ${CMAKE_SOURCE_DIR}/src/Devices
        ${CMAKE_SOURCE_DIR}/src/Logger
)

target_link_libraries(SystemControl
    PUBLIC
        Core
        Devices
        Logger
)
