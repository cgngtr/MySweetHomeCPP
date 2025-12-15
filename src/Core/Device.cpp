#include "Device.h"
#include <sstream>

namespace MySweetHome {

Device::Device(uint32_t id, const std::string& name, DeviceType type, const std::string& location)
    : m_id(id)
    , m_name(name)
    , m_type(type)
    , m_status(STATUS_OFF)
    , m_location(location)
    , m_isActive(true)
{
}

Device::~Device()
{
}

void Device::turnOn() {
    if (m_isActive) {
        m_status = STATUS_ON;
    }
}

void Device::turnOff() {
    if (!isCritical()) {
        m_status = STATUS_OFF;
    }
}

void Device::toggle() {
    if (m_status == STATUS_ON) {
        turnOff();
    } else {
        turnOn();
    }
}

uint32_t Device::getId() const {
    return m_id;
}

std::string Device::getName() const {
    return m_name;
}

DeviceType Device::getType() const {
    return m_type;
}

DeviceStatus Device::getStatus() const {
    return m_status;
}

std::string Device::getLocation() const {
    return m_location;
}

bool Device::isActive() const {
    return m_isActive;
}

void Device::setId(uint32_t id) {
    m_id = id;
}

void Device::setName(const std::string& name) {
    m_name = name;
}

void Device::setLocation(const std::string& location) {
    m_location = location;
}

void Device::setActive(bool active) {
    m_isActive = active;
    if (!active) {
        m_status = STATUS_INACTIVE;
    }
}

bool Device::isOn() const {
    return m_status == STATUS_ON;
}

std::string Device::getStatusString() const {
    switch (m_status) {
        case STATUS_ON:       return "On";
        case STATUS_OFF:      return "Off";
        case STATUS_STANDBY:  return "Standby";
        case STATUS_ERROR:    return "Error";
        case STATUS_INACTIVE: return "Inactive";
        default:              return "Unknown";
    }
}

std::string Device::getInfo() const {
    std::ostringstream oss;
    oss << "ID: " << m_id
        << " | Name: " << m_name
        << " | Location: " << (m_location.empty() ? "Not specified" : m_location)
        << " | Status: " << getStatusString();
    return oss.str();
}

bool Device::isCritical() const {
    return false;
}

void Device::setStatus(DeviceStatus status) {
    m_status = status;
}
void Device::addObserver(IObserver* observer) {
    if (observer) {
        m_observers.push_back(observer);
    }
}

void Device::removeObserver(IObserver* observer) {
    for (std::vector<IObserver*>::iterator it = m_observers.begin();
         it != m_observers.end(); ++it) {
        if (*it == observer) {
            m_observers.erase(it);
            break;
        }
    }
}

void Device::notifyObservers(const std::string& event, const std::string& message) {
    for (size_t i = 0; i < m_observers.size(); ++i) {
        m_observers[i]->onNotify(event, message);
    }
}

void Device::simulateFailure() {
    m_status = STATUS_ERROR;
    m_isActive = false;
    notifyObservers("DEVICE_FAILURE", m_name + " failure detected");
}

}
