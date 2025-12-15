#ifndef DEVICE_PROXY_H
#define DEVICE_PROXY_H

#include "Device.h"
#include <string>

namespace MySweetHome {
enum AccessLevel {
    ACCESS_READONLY = 0,
    ACCESS_NORMAL = 1,
    ACCESS_ADMIN = 2
};
class DeviceProxy : public Device {
public:
    DeviceProxy(Device* realDevice);
    virtual ~DeviceProxy();
    virtual void turnOn();
    virtual void turnOff();
    virtual Device* clone() const;
    virtual std::string getInfo() const;
    virtual std::string getStatusString() const;
    virtual bool isCritical() const;
    void setAccessLevel(AccessLevel level);
    AccessLevel getAccessLevel() const;
    bool checkAccess(const std::string& operation) const;
    Device* getRealDevice() const;
    void enableLogging(bool enable);
    bool isLoggingEnabled() const;

protected:
    Device* m_realDevice;
    AccessLevel m_accessLevel;
    bool m_loggingEnabled;

    void logAccess(const std::string& operation, bool allowed);
};
class CriticalDeviceProxy : public DeviceProxy {
public:
    CriticalDeviceProxy(Device* realDevice);
    virtual ~CriticalDeviceProxy();
    virtual void turnOff();
    bool adminOverrideTurnOff(const std::string& adminCode);
    void setAdminCode(const std::string& code);
    virtual bool isCritical() const;

private:
    std::string m_adminCode;
    bool verifyAdminCode(const std::string& code) const;
};
class LazyDeviceProxy : public Device {
public:
    LazyDeviceProxy(DeviceType type,
                   const std::string& name,
                   const std::string& location);
    virtual ~LazyDeviceProxy();
    virtual void turnOn();
    virtual void turnOff();
    virtual Device* clone() const;
    virtual std::string getInfo() const;
    virtual std::string getStatusString() const;
    virtual bool isCritical() const;
    bool isInitialized() const;
    void forceInitialize();

private:
    Device* m_realDevice;
    DeviceType m_pendingType;
    std::string m_pendingName;
    std::string m_pendingLocation;

    void ensureInitialized();
    Device* createRealDevice();
};
class LoggingDeviceProxy : public DeviceProxy {
public:
    LoggingDeviceProxy(Device* realDevice);
    virtual ~LoggingDeviceProxy();

    virtual void turnOn();
    virtual void turnOff();
    int getTurnOnCount() const;
    int getTurnOffCount() const;
    void resetCounts();

private:
    int m_turnOnCount;
    int m_turnOffCount;
};
class CachingDeviceProxy : public DeviceProxy {
public:
    CachingDeviceProxy(Device* realDevice);
    virtual ~CachingDeviceProxy();

    virtual std::string getInfo() const;
    virtual std::string getStatusString() const;
    void invalidateCache();
    void setCacheDuration(int seconds);

private:
    mutable std::string m_cachedInfo;
    mutable std::string m_cachedStatus;
    mutable bool m_infoCacheValid;
    mutable bool m_statusCacheValid;
    int m_cacheDuration;
};
class DeviceProxyFactory {
public:
    enum ProxyType {
        PROXY_PROTECTION,
        PROXY_CRITICAL,
        PROXY_LOGGING,
        PROXY_CACHING
    };
    static DeviceProxy* createProxy(Device* device, ProxyType type);
    static DeviceProxy* createAutoProxy(Device* device);
    static Device* wrapIfCritical(Device* device);
};

}

#endif
