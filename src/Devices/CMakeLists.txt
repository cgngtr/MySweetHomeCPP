add_library(Devices
    Light.cpp
    Camera.cpp
    Detector.cpp
    TV.cpp
    Alarm.cpp
    SoundSystem.cpp
    DetectorFactory.cpp
    DetectionStrategy.cpp
)

target_include_directories(Devices
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_SOURCE_DIR}/include
        ${CMAKE_SOURCE_DIR}/src/Core
)

target_link_libraries(Devices
    PUBLIC
        Core
)
