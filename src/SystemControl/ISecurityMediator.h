#ifndef ISECURITY_MEDIATOR_H
#define ISECURITY_MEDIATOR_H

#include <string>

namespace MySweetHome {

class ISecurityColleague;
class ISecurityMediator {
public:
    virtual ~ISecurityMediator() {}
    virtual void registerColleague(ISecurityColleague* colleague) = 0;
    virtual void unregisterColleague(ISecurityColleague* colleague) = 0;
    virtual void notify(ISecurityColleague* sender, const std::string& event) = 0;
    virtual void onMotionDetected(ISecurityColleague* camera) = 0;
    virtual void onSmokeDetected(ISecurityColleague* detector) = 0;
    virtual void onGasDetected(ISecurityColleague* detector) = 0;
    virtual void onAlarmAcknowledged() = 0;
    virtual void onAlarmTriggered(ISecurityColleague* alarm) = 0;
    virtual bool isSequenceActive() const = 0;
    virtual void resetSequence() = 0;
};
class ISecurityColleague {
public:
    virtual ~ISecurityColleague() {}
    virtual void setMediator(ISecurityMediator* mediator) = 0;
    virtual std::string getColleagueType() const = 0;
    virtual void onSecurityCommand(const std::string& command) = 0;
};
const std::string EVENT_MOTION_DETECTED = "MOTION_DETECTED";
const std::string EVENT_SMOKE_DETECTED = "SMOKE_DETECTED";
const std::string EVENT_GAS_DETECTED = "GAS_DETECTED";
const std::string EVENT_ALARM_TRIGGERED = "ALARM_TRIGGERED";
const std::string EVENT_ALARM_ACKNOWLEDGED = "ALARM_ACKNOWLEDGED";
const std::string EVENT_LIGHTS_ON = "LIGHTS_ON";
const std::string EVENT_LIGHTS_OFF = "LIGHTS_OFF";
const std::string EVENT_LIGHTS_BLINK = "LIGHTS_BLINK";
const std::string EVENT_CALL_POLICE = "CALL_POLICE";
const std::string EVENT_CALL_FIRE = "CALL_FIRE";
const std::string EVENT_RECORDING_START = "RECORDING_START";
const std::string EVENT_RECORDING_STOP = "RECORDING_STOP";
const std::string CMD_ACTIVATE_ALARM = "ACTIVATE_ALARM";
const std::string CMD_DEACTIVATE_ALARM = "DEACTIVATE_ALARM";
const std::string CMD_TURN_ON_LIGHTS = "TURN_ON_LIGHTS";
const std::string CMD_TURN_OFF_LIGHTS = "TURN_OFF_LIGHTS";
const std::string CMD_START_BLINKING = "START_BLINKING";
const std::string CMD_STOP_BLINKING = "STOP_BLINKING";
const std::string CMD_START_RECORDING = "START_RECORDING";
const std::string CMD_STOP_RECORDING = "STOP_RECORDING";
const std::string CMD_CALL_POLICE = "CALL_POLICE";
const std::string CMD_CALL_FIRE_STATION = "CALL_FIRE_STATION";

}

#endif
