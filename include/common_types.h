#ifndef COMMON_TYPES_H
#define COMMON_TYPES_H

#include <string>

namespace MySweetHome {
typedef unsigned int uint32_t;
typedef unsigned short uint16_t;
typedef unsigned char uint8_t;
enum DeviceType {
    DEVICE_LIGHT,
    DEVICE_CAMERA,
    DEVICE_SMOKE_DETECTOR,
    DEVICE_GAS_DETECTOR,
    DEVICE_TV,
    DEVICE_ALARM,
    DEVICE_SOUND_SYSTEM
};
enum DeviceStatus {
    STATUS_OFF,
    STATUS_ON,
    STATUS_STANDBY,
    STATUS_ERROR,
    STATUS_INACTIVE
};
enum SystemMode {
    MODE_NORMAL,
    MODE_EVENING,
    MODE_PARTY,
    MODE_CINEMA
};
enum SystemState {
    STATE_NORMAL,
    STATE_HIGH_PERFORMANCE,
    STATE_LOW_POWER,
    STATE_SLEEP
};
enum LogLevel {
    LOG_DEBUG,
    LOG_INFO,
    LOG_WARNING,
    LOG_ERROR,
    LOG_CRITICAL
};
enum AlarmType {
    ALARM_FIRE,
    ALARM_GAS_LEAK,
    ALARM_INTRUSION,
    ALARM_SYSTEM_ERROR
};
enum NotificationType {
    NOTIFY_LOG,
    NOTIFY_ALARM,
    NOTIFY_SMS
};
struct DeviceInfo {
    uint32_t id;
    std::string name;
    DeviceType type;
    DeviceStatus status;
    std::string location;
};
const uint32_t MAX_DEVICES = 100;
const uint32_t MAX_LOG_ENTRIES = 1000;

}

#endif
