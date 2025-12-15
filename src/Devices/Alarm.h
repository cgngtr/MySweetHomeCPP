#ifndef ALARM_H
#define ALARM_H

#include "Device.h"

namespace MySweetHome {

enum AlarmState {
    ALARM_DISARMED,
    ALARM_ARMED_HOME,
    ALARM_ARMED_AWAY,
    ALARM_TRIGGERED
};

class Alarm : public Device {
public:
    Alarm(uint32_t id, const std::string& name, const std::string& location);
    virtual ~Alarm();
    void arm();
    void armHome();
    void armAway();
    void disarm(const std::string& code);
    void trigger(AlarmType type);
    void silence();
    bool setPin(const std::string& oldPin, const std::string& newPin);
    bool verifyPin(const std::string& pin) const;
    AlarmState getAlarmState() const;
    bool isArmed() const;
    bool isTriggered() const;
    bool isSirenActive() const;
    virtual Device* clone() const;
    virtual void turnOn();
    virtual void turnOff();
    virtual std::string getInfo() const;
    virtual bool isCritical() const;

private:
    AlarmState m_alarmState;
    std::string m_pinCode;
    AlarmType m_lastTriggerType;
    bool m_sirenActive;
};

}

#endif
