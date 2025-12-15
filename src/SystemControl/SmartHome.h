#ifndef SMART_HOME_H
#define SMART_HOME_H

#include <vector>
#include <string>
#include "Device.h"
#include "StateManager.h"
#include "ModeManager.h"
#include "IObserver.h"
#include "common_types.h"

namespace MySweetHome {

class IDetectorFactory;
class SecurityManager;
class SmartHome {
public:
    SmartHome();
    ~SmartHome();
    bool addDevice(Device* device);
    bool removeDevice(uint32_t id);
    Device* getDevice(uint32_t id) const;
    std::vector<Device*> getAllDevices() const;
    std::vector<Device*> getDevicesByType(DeviceType type) const;
    std::vector<Device*> getDevicesByLocation(const std::string& location) const;
    Device* addLight(const std::string& name, const std::string& location);
    Device* addChinaLight(const std::string& name, const std::string& location);
    Device* addCamera(const std::string& name, const std::string& location);
    Device* addSamsungTV(const std::string& location);
    Device* addLGTV(const std::string& location);
    Device* addSoundSystem(const std::string& name, const std::string& location);
    Device* addAlarm(const std::string& name, const std::string& location);
    void addDetectorPair(const std::string& name, const std::string& location);
    bool addDeviceWithClone(Device* prototype, int count);
    bool addClonedDevice(Device* clone);
    void setMode(SystemMode mode);
    SystemMode getCurrentMode() const;
    std::string getCurrentModeString() const;
    void setState(SystemState state);
    SystemState getCurrentState() const;
    std::string getCurrentStateString() const;
    bool goToPreviousState();
    bool goToNextState();
    void turnAllOff();
    void turnAllOn();
    void turnOffByType(DeviceType type);
    void turnOnByType(DeviceType type);
    void turnOffByLocation(const std::string& location);
    void turnOnByLocation(const std::string& location);
    bool powerOnDevice(uint32_t id);
    bool powerOffDevice(uint32_t id);
    size_t getDeviceCount() const;
    size_t getActiveDeviceCount() const;
    void update();
    SecurityManager* getSecurityManager();
    void setNotificationPreference(NotificationType type);
    NotificationManager* getNotificationManager();

private:
    uint32_t generateDeviceId();
    void cleanupDevices();

    std::vector<Device*> m_devices;
    StateManager m_stateManager;
    ModeManager m_modeManager;
    IDetectorFactory* m_detectorFactory;
    SecurityManager* m_securityManager;
    NotificationManager* m_notificationManager;
    uint32_t m_nextDeviceId;
};

}

#endif
