#ifndef IDEVICE_IMPL_H
#define IDEVICE_IMPL_H

#include <string>

namespace MySweetHome {
class IDeviceImpl {
public:
    virtual ~IDeviceImpl() {}
    virtual void powerOn() = 0;
    virtual void powerOff() = 0;
    virtual bool isPowered() const = 0;
    virtual int getHardwareStatus() const = 0;
    virtual std::string getImplType() const = 0;
    virtual std::string getImplInfo() const = 0;
    virtual bool isConnected() const = 0;
    virtual bool connect() = 0;
    virtual void disconnect() = 0;
};

}

#endif
