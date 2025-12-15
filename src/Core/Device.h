#ifndef DEVICE_H
#define DEVICE_H

#include <string>
#include <vector>
#include "common_types.h"
#include "IObserver.h"

namespace MySweetHome {

class Device : public ISubject {
public:
    Device(uint32_t id, const std::string& name, DeviceType type, const std::string& location);
    virtual ~Device();
    virtual void turnOn();
    virtual void turnOff();
    virtual void toggle();
    virtual Device* clone() const = 0;
    uint32_t getId() const;
    std::string getName() const;
    DeviceType getType() const;
    DeviceStatus getStatus() const;
    std::string getLocation() const;
    bool isActive() const;
    void setId(uint32_t id);
    void setName(const std::string& name);
    void setLocation(const std::string& location);
    void setActive(bool active);
    bool isOn() const;
    virtual std::string getStatusString() const;
    virtual std::string getInfo() const;
    virtual bool isCritical() const;
    virtual void addObserver(IObserver* observer);
    virtual void removeObserver(IObserver* observer);
    virtual void notifyObservers(const std::string& event, const std::string& message);
    void simulateFailure();

protected:
    void setStatus(DeviceStatus status);

private:
    uint32_t m_id;
    std::string m_name;
    DeviceType m_type;
    DeviceStatus m_status;
    std::string m_location;
    bool m_isActive;
    std::vector<IObserver*> m_observers;
};

}

#endif
