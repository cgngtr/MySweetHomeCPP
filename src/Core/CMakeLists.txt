add_library(Core
    Device.cpp
    IObserver.cpp
    DeviceIterator.cpp
    DeviceCollection.cpp
    LightAdapter.cpp
    NotificationHandler.cpp
    DeviceProxy.cpp
    DeviceImpl.cpp
)

target_include_directories(Core
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_SOURCE_DIR}/include
        ${CMAKE_SOURCE_DIR}/src/Logger
        ${CMAKE_SOURCE_DIR}/src/Devices
)
