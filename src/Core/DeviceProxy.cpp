#include "DeviceProxy.h"
#include "Logger.h"
#include "Light.h"
#include "Camera.h"
#include <sstream>

namespace MySweetHome {
DeviceProxy::DeviceProxy(Device* realDevice)
    : Device(realDevice ? realDevice->getId() : 0,
             realDevice ? realDevice->getName() : "Proxy",
             realDevice ? realDevice->getType() : DEVICE_LIGHT,
             realDevice ? realDevice->getLocation() : "")
    , m_realDevice(realDevice)
    , m_accessLevel(ACCESS_NORMAL)
    , m_loggingEnabled(true)
{
}

DeviceProxy::~DeviceProxy()
{
}

void DeviceProxy::turnOn()
{
    if (!m_realDevice) return;

    if (checkAccess("turnOn")) {
        logAccess("turnOn", true);
        m_realDevice->turnOn();
        setStatus(m_realDevice->getStatus());
    } else {
        logAccess("turnOn", false);
    }
}

void DeviceProxy::turnOff()
{
    if (!m_realDevice) return;

    if (m_realDevice->isCritical() && m_accessLevel < ACCESS_ADMIN) {
        logAccess("turnOff", false);
        Logger::getInstance().warning("Attempted to turn off critical device: " +
                                     m_realDevice->getName());
        return;
    }

    if (checkAccess("turnOff")) {
        logAccess("turnOff", true);
        m_realDevice->turnOff();
        setStatus(m_realDevice->getStatus());
    } else {
        logAccess("turnOff", false);
    }
}

Device* DeviceProxy::clone() const
{
    if (!m_realDevice) return 0;

    Device* clonedDevice = m_realDevice->clone();
    DeviceProxy* clonedProxy = new DeviceProxy(clonedDevice);
    clonedProxy->m_accessLevel = m_accessLevel;
    clonedProxy->m_loggingEnabled = m_loggingEnabled;
    return clonedProxy;
}

std::string DeviceProxy::getInfo() const
{
    if (!m_realDevice) return "Invalid Proxy";
    return "[Proxy] " + m_realDevice->getInfo();
}

std::string DeviceProxy::getStatusString() const
{
    if (!m_realDevice) return "Invalid";
    return m_realDevice->getStatusString();
}

bool DeviceProxy::isCritical() const
{
    if (!m_realDevice) return false;
    return m_realDevice->isCritical();
}

void DeviceProxy::setAccessLevel(AccessLevel level)
{
    m_accessLevel = level;
}

AccessLevel DeviceProxy::getAccessLevel() const
{
    return m_accessLevel;
}

bool DeviceProxy::checkAccess(const std::string& operation) const
{
    if (m_accessLevel == ACCESS_READONLY) {
        return operation == "getInfo" || operation == "getStatus";
    }
    return true;
}

Device* DeviceProxy::getRealDevice() const
{
    return m_realDevice;
}

void DeviceProxy::enableLogging(bool enable)
{
    m_loggingEnabled = enable;
}

bool DeviceProxy::isLoggingEnabled() const
{
    return m_loggingEnabled;
}

void DeviceProxy::logAccess(const std::string& operation, bool allowed)
{
    if (!m_loggingEnabled) return;

    std::ostringstream oss;
    oss << "Proxy access: " << operation
        << " on " << (m_realDevice ? m_realDevice->getName() : "null")
        << " - " << (allowed ? "ALLOWED" : "DENIED");

    if (allowed) {
        Logger::getInstance().debug(oss.str());
    } else {
        Logger::getInstance().warning(oss.str());
    }
}
CriticalDeviceProxy::CriticalDeviceProxy(Device* realDevice)
    : DeviceProxy(realDevice)
    , m_adminCode("admin123")
{
}

CriticalDeviceProxy::~CriticalDeviceProxy()
{
}

void CriticalDeviceProxy::turnOff()
{
    logAccess("turnOff", false);
    Logger::getInstance().warning("Critical device cannot be turned off: " +
                                 (m_realDevice ? m_realDevice->getName() : "null"));
}

bool CriticalDeviceProxy::adminOverrideTurnOff(const std::string& adminCode)
{
    if (verifyAdminCode(adminCode)) {
        logAccess("adminOverrideTurnOff", true);
        if (m_realDevice) {
            m_realDevice->Device::turnOff();
            setStatus(m_realDevice->getStatus());
        }
        Logger::getInstance().warning("Admin override: Critical device turned off");
        return true;
    }

    logAccess("adminOverrideTurnOff", false);
    Logger::getInstance().error("Invalid admin code for critical device override");
    return false;
}

void CriticalDeviceProxy::setAdminCode(const std::string& code)
{
    m_adminCode = code;
}

bool CriticalDeviceProxy::isCritical() const
{
    return true;
}

bool CriticalDeviceProxy::verifyAdminCode(const std::string& code) const
{
    return code == m_adminCode;
}
LazyDeviceProxy::LazyDeviceProxy(DeviceType type,
                                 const std::string& name,
                                 const std::string& location)
    : Device(0, name, type, location)
    , m_realDevice(0)
    , m_pendingType(type)
    , m_pendingName(name)
    , m_pendingLocation(location)
{
}

LazyDeviceProxy::~LazyDeviceProxy()
{
    delete m_realDevice;
}

void LazyDeviceProxy::turnOn()
{
    ensureInitialized();
    if (m_realDevice) {
        m_realDevice->turnOn();
        setStatus(m_realDevice->getStatus());
    }
}

void LazyDeviceProxy::turnOff()
{
    ensureInitialized();
    if (m_realDevice) {
        m_realDevice->turnOff();
        setStatus(m_realDevice->getStatus());
    }
}

Device* LazyDeviceProxy::clone() const
{
    LazyDeviceProxy* cloned = new LazyDeviceProxy(m_pendingType,
                                                   m_pendingName,
                                                   m_pendingLocation);
    return cloned;
}

std::string LazyDeviceProxy::getInfo() const
{
    if (m_realDevice) {
        return "[Lazy] " + m_realDevice->getInfo();
    }
    return "[Lazy-NotLoaded] " + m_pendingName;
}

std::string LazyDeviceProxy::getStatusString() const
{
    if (m_realDevice) {
        return m_realDevice->getStatusString();
    }
    return "Not Loaded";
}

bool LazyDeviceProxy::isCritical() const
{
    if (m_realDevice) {
        return m_realDevice->isCritical();
    }
    return m_pendingType == DEVICE_CAMERA ||
           m_pendingType == DEVICE_SMOKE_DETECTOR ||
           m_pendingType == DEVICE_GAS_DETECTOR ||
           m_pendingType == DEVICE_ALARM;
}

bool LazyDeviceProxy::isInitialized() const
{
    return m_realDevice != 0;
}

void LazyDeviceProxy::forceInitialize()
{
    ensureInitialized();
}

void LazyDeviceProxy::ensureInitialized()
{
    if (!m_realDevice) {
        Logger::getInstance().debug("Lazy loading device: " + m_pendingName);
        m_realDevice = createRealDevice();
    }
}

Device* LazyDeviceProxy::createRealDevice()
{
    uint32_t deviceId = getId();
    switch (m_pendingType) {
        case DEVICE_LIGHT:
            return new Light(deviceId, m_pendingName, m_pendingLocation);
        case DEVICE_CAMERA:
            return new Camera(deviceId, m_pendingName, m_pendingLocation);
        default:
            return new Light(deviceId, m_pendingName, m_pendingLocation);
    }
}
LoggingDeviceProxy::LoggingDeviceProxy(Device* realDevice)
    : DeviceProxy(realDevice)
    , m_turnOnCount(0)
    , m_turnOffCount(0)
{
    m_loggingEnabled = true;
}

LoggingDeviceProxy::~LoggingDeviceProxy()
{
}

void LoggingDeviceProxy::turnOn()
{
    ++m_turnOnCount;
    std::ostringstream oss;
    oss << "Device turnOn #" << m_turnOnCount << ": "
        << (m_realDevice ? m_realDevice->getName() : "null");
    Logger::getInstance().info(oss.str());

    DeviceProxy::turnOn();
}

void LoggingDeviceProxy::turnOff()
{
    ++m_turnOffCount;
    std::ostringstream oss;
    oss << "Device turnOff #" << m_turnOffCount << ": "
        << (m_realDevice ? m_realDevice->getName() : "null");
    Logger::getInstance().info(oss.str());

    DeviceProxy::turnOff();
}

int LoggingDeviceProxy::getTurnOnCount() const
{
    return m_turnOnCount;
}

int LoggingDeviceProxy::getTurnOffCount() const
{
    return m_turnOffCount;
}

void LoggingDeviceProxy::resetCounts()
{
    m_turnOnCount = 0;
    m_turnOffCount = 0;
}
CachingDeviceProxy::CachingDeviceProxy(Device* realDevice)
    : DeviceProxy(realDevice)
    , m_infoCacheValid(false)
    , m_statusCacheValid(false)
    , m_cacheDuration(60)
{
}

CachingDeviceProxy::~CachingDeviceProxy()
{
}

std::string CachingDeviceProxy::getInfo() const
{
    if (!m_infoCacheValid && m_realDevice) {
        m_cachedInfo = m_realDevice->getInfo();
        m_infoCacheValid = true;
    }
    return "[Cached] " + m_cachedInfo;
}

std::string CachingDeviceProxy::getStatusString() const
{
    if (!m_statusCacheValid && m_realDevice) {
        m_cachedStatus = m_realDevice->getStatusString();
        m_statusCacheValid = true;
    }
    return m_cachedStatus;
}

void CachingDeviceProxy::invalidateCache()
{
    m_infoCacheValid = false;
    m_statusCacheValid = false;
}

void CachingDeviceProxy::setCacheDuration(int seconds)
{
    if (seconds > 0) {
        m_cacheDuration = seconds;
    }
}
DeviceProxy* DeviceProxyFactory::createProxy(Device* device, ProxyType type)
{
    if (!device) return 0;

    switch (type) {
        case PROXY_PROTECTION:
            return new DeviceProxy(device);
        case PROXY_CRITICAL:
            return new CriticalDeviceProxy(device);
        case PROXY_LOGGING:
            return new LoggingDeviceProxy(device);
        case PROXY_CACHING:
            return new CachingDeviceProxy(device);
        default:
            return new DeviceProxy(device);
    }
}

DeviceProxy* DeviceProxyFactory::createAutoProxy(Device* device)
{
    if (!device) return 0;
    if (device->isCritical()) {
        return new CriticalDeviceProxy(device);
    }

    return new DeviceProxy(device);
}

Device* DeviceProxyFactory::wrapIfCritical(Device* device)
{
    if (!device) return 0;

    if (device->isCritical()) {
        return new CriticalDeviceProxy(device);
    }

    return device;
}

}
