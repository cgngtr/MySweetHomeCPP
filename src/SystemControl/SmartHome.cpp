#include "SmartHome.h"
#include "SecurityManager.h"
#include "Light.h"
#include "Camera.h"
#include "Detector.h"
#include "DetectorFactory.h"
#include "TV.h"
#include "Alarm.h"
#include "SoundSystem.h"
#include "Logger.h"
#include <algorithm>
#include <sstream>

namespace MySweetHome {

SmartHome::SmartHome()
    : m_detectorFactory(0)
    , m_securityManager(0)
    , m_notificationManager(0)
    , m_nextDeviceId(1)
{
    m_detectorFactory = new StandardDetectorFactory();
    m_notificationManager = new NotificationManager();
    Logger::getInstance().info("SmartHome system started.");
}

SmartHome::~SmartHome() {
    cleanupDevices();
    delete m_securityManager;
    delete m_notificationManager;
    delete m_detectorFactory;
    Logger::getInstance().info("SmartHome system shutdown.");
}

void SmartHome::cleanupDevices() {
    for (size_t i = 0; i < m_devices.size(); ++i) {
        delete m_devices[i];
    }
    m_devices.clear();
}

bool SmartHome::addDevice(Device* device) {
    if (!device) {
        return false;
    }

    if (m_devices.size() >= MAX_DEVICES) {
        Logger::getInstance().warning("Maximum device count reached!");
        return false;
    }

    m_devices.push_back(device);
    Logger::getInstance().info("Device added: " + device->getName());
    return true;
}

bool SmartHome::removeDevice(uint32_t id) {
    for (size_t i = 0; i < m_devices.size(); ++i) {
        if (m_devices[i]->getId() == id) {
            Logger::getInstance().info("Device removed: " + m_devices[i]->getName());
            delete m_devices[i];
            m_devices.erase(m_devices.begin() + i);
            return true;
        }
    }
    return false;
}

Device* SmartHome::getDevice(uint32_t id) const {
    for (size_t i = 0; i < m_devices.size(); ++i) {
        if (m_devices[i]->getId() == id) {
            return m_devices[i];
        }
    }
    return 0;
}

std::vector<Device*> SmartHome::getAllDevices() const {
    return m_devices;
}

std::vector<Device*> SmartHome::getDevicesByType(DeviceType type) const {
    std::vector<Device*> result;
    for (size_t i = 0; i < m_devices.size(); ++i) {
        if (m_devices[i]->getType() == type) {
            result.push_back(m_devices[i]);
        }
    }
    return result;
}

std::vector<Device*> SmartHome::getDevicesByLocation(const std::string& location) const {
    std::vector<Device*> result;
    for (size_t i = 0; i < m_devices.size(); ++i) {
        if (m_devices[i]->getLocation() == location) {
            result.push_back(m_devices[i]);
        }
    }
    return result;
}

Device* SmartHome::addLight(const std::string& name, const std::string& location) {
    Light* light = new Light(generateDeviceId(), name, location);
    if (addDevice(light)) {
        return light;
    }
    delete light;
    return 0;
}

Device* SmartHome::addChinaLight(const std::string& name, const std::string& location) {
    ChinaLight* light = new ChinaLight(generateDeviceId(), name, location);
    if (addDevice(light)) {
        return light;
    }
    delete light;
    return 0;
}

Device* SmartHome::addCamera(const std::string& name, const std::string& location) {
    Camera* camera = new Camera(generateDeviceId(), name, location);
    if (addDevice(camera)) {
        camera->turnOn();
        return camera;
    }
    delete camera;
    return 0;
}

Device* SmartHome::addSamsungTV(const std::string& location) {
    SamsungTV* tv = new SamsungTV(generateDeviceId(), location);
    if (addDevice(tv)) {
        return tv;
    }
    delete tv;
    return 0;
}

Device* SmartHome::addLGTV(const std::string& location) {
    LGTV* tv = new LGTV(generateDeviceId(), location);
    if (addDevice(tv)) {
        return tv;
    }
    delete tv;
    return 0;
}

Device* SmartHome::addSoundSystem(const std::string& name, const std::string& location) {
    SoundSystem* sound = new SoundSystem(generateDeviceId(), name, location);
    if (addDevice(sound)) {
        return sound;
    }
    delete sound;
    return 0;
}

Device* SmartHome::addAlarm(const std::string& name, const std::string& location) {
    Alarm* alarm = new Alarm(generateDeviceId(), name, location);
    if (addDevice(alarm)) {
        alarm->turnOn();
        return alarm;
    }
    delete alarm;
    return 0;
}

void SmartHome::addDetectorPair(const std::string& name, const std::string& location) {
    IDetectorFactory::DetectorPair pair = m_detectorFactory->createDetectorPair(
        generateDeviceId(), name, location);

    if (pair.smoke) {
        addDevice(pair.smoke);
        pair.smoke->turnOn();
    }
    if (pair.gas) {
        m_nextDeviceId++;
        addDevice(pair.gas);
        pair.gas->turnOn();
    }
}

bool SmartHome::addDeviceWithClone(Device* prototype, int count) {
    if (!prototype || count <= 0) {
        return false;
    }

    for (int i = 0; i < count; ++i) {
        Device* clone = prototype->clone();
        clone->setId(generateDeviceId());
        if (!addDevice(clone)) {
            delete clone;
            return false;
        }
    }
    return true;
}

bool SmartHome::addClonedDevice(Device* clone) {
    if (!clone) {
        return false;
    }
    clone->setId(generateDeviceId());
    return addDevice(clone);
}

void SmartHome::setMode(SystemMode mode) {
    m_modeManager.setMode(mode);
    m_modeManager.applyModeToDevices(m_devices);
}

SystemMode SmartHome::getCurrentMode() const {
    return m_modeManager.getCurrentMode();
}

std::string SmartHome::getCurrentModeString() const {
    return m_modeManager.getCurrentModeString();
}

void SmartHome::setState(SystemState state) {
    m_stateManager.setState(state);
}

SystemState SmartHome::getCurrentState() const {
    return m_stateManager.getCurrentState();
}

std::string SmartHome::getCurrentStateString() const {
    return m_stateManager.getCurrentStateString();
}

bool SmartHome::goToPreviousState() {
    return m_stateManager.goToPreviousState();
}

bool SmartHome::goToNextState() {
    return m_stateManager.goToNextState();
}

void SmartHome::turnAllOff() {
    for (size_t i = 0; i < m_devices.size(); ++i) {
        if (!m_devices[i]->isCritical()) {
            m_devices[i]->turnOff();
        }
    }
    Logger::getInstance().info("All devices turned off (except critical devices).");
}

void SmartHome::turnAllOn() {
    for (size_t i = 0; i < m_devices.size(); ++i) {
        m_devices[i]->turnOn();
    }
    Logger::getInstance().info("All devices turned on.");
}

void SmartHome::turnOffByType(DeviceType type) {
    for (size_t i = 0; i < m_devices.size(); ++i) {
        if (m_devices[i]->getType() == type && !m_devices[i]->isCritical()) {
            m_devices[i]->turnOff();
        }
    }
}

void SmartHome::turnOnByType(DeviceType type) {
    for (size_t i = 0; i < m_devices.size(); ++i) {
        if (m_devices[i]->getType() == type) {
            m_devices[i]->turnOn();
        }
    }
}

void SmartHome::turnOffByLocation(const std::string& location) {
    for (size_t i = 0; i < m_devices.size(); ++i) {
        if (m_devices[i]->getLocation() == location && !m_devices[i]->isCritical()) {
            m_devices[i]->turnOff();
        }
    }
}

void SmartHome::turnOnByLocation(const std::string& location) {
    for (size_t i = 0; i < m_devices.size(); ++i) {
        if (m_devices[i]->getLocation() == location) {
            m_devices[i]->turnOn();
        }
    }
}

bool SmartHome::powerOnDevice(uint32_t id) {
    Device* device = getDevice(id);
    if (device) {
        device->turnOn();
        std::ostringstream oss;
        oss << device->getName() << " powered on.";
        Logger::getInstance().info(oss.str());
        return true;
    }
    return false;
}

bool SmartHome::powerOffDevice(uint32_t id) {
    Device* device = getDevice(id);
    if (device) {
        if (device->isCritical()) {
            Logger::getInstance().warning("Critical device cannot be powered off: " + device->getName());
            return false;
        }
        device->turnOff();
        std::ostringstream oss;
        oss << device->getName() << " powered off.";
        Logger::getInstance().info(oss.str());
        return true;
    }
    return false;
}

size_t SmartHome::getDeviceCount() const {
    return m_devices.size();
}

size_t SmartHome::getActiveDeviceCount() const {
    size_t count = 0;
    for (size_t i = 0; i < m_devices.size(); ++i) {
        if (m_devices[i]->isOn()) {
            count++;
        }
    }
    return count;
}

void SmartHome::update() {
}

SecurityManager* SmartHome::getSecurityManager() {
    if (!m_securityManager) {
        m_securityManager = new SecurityManager(this);
    }
    return m_securityManager;
}

void SmartHome::setNotificationPreference(NotificationType type) {
    if (m_notificationManager) {
        m_notificationManager->setNotificationType(type);
    }
}

NotificationManager* SmartHome::getNotificationManager() {
    return m_notificationManager;
}

uint32_t SmartHome::generateDeviceId() {
    return m_nextDeviceId++;
}

}
