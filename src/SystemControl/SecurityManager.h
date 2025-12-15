#ifndef SECURITY_MANAGER_H
#define SECURITY_MANAGER_H

#include <string>
#include <ctime>
#include "common_types.h"

namespace MySweetHome {

class SmartHome;
class Alarm;
class SecurityManager {
public:
    SecurityManager(SmartHome* smartHome);
    ~SecurityManager();
    void handleMotionDetected();
    void handleSmokeDetected();
    void handleGasDetected();
    bool waitForAcknowledgment(int timeoutSeconds);
    void acknowledgeAlarm();
    bool isAlarmAcknowledged() const;
    void callPolice();
    void callFireStation();
    void startLightBlinking();
    void stopLightBlinking();
    void blinkLightsOnce();
    bool isSequenceActive() const;
    void resetSequence();

private:
    void activateAlarm(AlarmType type);
    void deactivateAlarm();
    void turnOnAllLights();
    void turnOffAllLights();
    void sleepSeconds(int seconds);
    void sleepMilliseconds(int milliseconds);
    bool checkForKeyPress();
    void handleFireGasSequence(AlarmType type, const std::string& detectorName);

    SmartHome* m_smartHome;
    Alarm* m_alarm;
    bool m_alarmAcknowledged;
    bool m_sequenceActive;
    bool m_isBlinking;
};

}

#endif
