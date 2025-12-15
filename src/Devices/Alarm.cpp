#include "Alarm.h"
#include <sstream>

namespace MySweetHome {

Alarm::Alarm(uint32_t id, const std::string& name, const std::string& location)
    : Device(id, name, DEVICE_ALARM, location)
    , m_alarmState(ALARM_DISARMED)
    , m_pinCode("1234")
    , m_lastTriggerType(ALARM_INTRUSION)
    , m_sirenActive(false)
{
}

Alarm::~Alarm()
{
}

void Alarm::arm() {
    armAway();
}

void Alarm::armHome() {
    if (isOn() && m_alarmState == ALARM_DISARMED) {
        m_alarmState = ALARM_ARMED_HOME;
    }
}

void Alarm::armAway() {
    if (isOn() && m_alarmState == ALARM_DISARMED) {
        m_alarmState = ALARM_ARMED_AWAY;
    }
}

void Alarm::disarm(const std::string& code) {
    if (verifyPin(code)) {
        m_alarmState = ALARM_DISARMED;
        m_sirenActive = false;
    }
}

void Alarm::trigger(AlarmType type) {
    if (isArmed()) {
        m_alarmState = ALARM_TRIGGERED;
        m_lastTriggerType = type;
        m_sirenActive = true;
    }
}

void Alarm::silence() {
    m_sirenActive = false;
}

bool Alarm::setPin(const std::string& oldPin, const std::string& newPin) {
    if (verifyPin(oldPin) && newPin.length() >= 4) {
        m_pinCode = newPin;
        return true;
    }
    return false;
}

bool Alarm::verifyPin(const std::string& pin) const {
    return pin == m_pinCode;
}

AlarmState Alarm::getAlarmState() const {
    return m_alarmState;
}

bool Alarm::isArmed() const {
    return m_alarmState == ALARM_ARMED_HOME ||
           m_alarmState == ALARM_ARMED_AWAY;
}

bool Alarm::isTriggered() const {
    return m_alarmState == ALARM_TRIGGERED;
}

bool Alarm::isSirenActive() const {
    return m_sirenActive;
}

Device* Alarm::clone() const {
    Alarm* newAlarm = new Alarm(getId(), getName(), getLocation());
    return newAlarm;
}

void Alarm::turnOn() {
    Device::turnOn();
}

void Alarm::turnOff() {
    m_alarmState = ALARM_DISARMED;
    m_sirenActive = false;
}

std::string Alarm::getInfo() const {
    std::ostringstream oss;
    oss << Device::getInfo() << " | Alarm Durumu: ";

    switch (m_alarmState) {
        case ALARM_DISARMED:
            oss << "Devre Disi";
            break;
        case ALARM_ARMED_HOME:
            oss << "Evde Mod";
            break;
        case ALARM_ARMED_AWAY:
            oss << "Uzakta Mod";
            break;
        case ALARM_TRIGGERED:
            oss << "TETIKLENDI!";
            break;
    }

    if (m_sirenActive) {
        oss << " | Siren: AKTIF!";
    }

    return oss.str();
}

bool Alarm::isCritical() const {
    return true;
}

}
