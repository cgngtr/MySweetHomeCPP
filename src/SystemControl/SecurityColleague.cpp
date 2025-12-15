#include "SecurityColleague.h"
#include "Alarm.h"
#include "Light.h"
#include "Camera.h"
#include "Detector.h"
#include "Logger.h"
#include <algorithm>
#include <iostream>

namespace MySweetHome {
BaseSecurityColleague::BaseSecurityColleague()
    : m_mediator(0)
{
}

BaseSecurityColleague::~BaseSecurityColleague()
{
}

void BaseSecurityColleague::setMediator(ISecurityMediator* mediator)
{
    m_mediator = mediator;
}

void BaseSecurityColleague::notifyMediator(const std::string& event)
{
    if (m_mediator) {
        m_mediator->notify(this, event);
    }
}
AlarmColleague::AlarmColleague(Alarm* alarm)
    : m_alarm(alarm)
    , m_isActive(false)
{
}

AlarmColleague::~AlarmColleague()
{
}

std::string AlarmColleague::getColleagueType() const
{
    return "AlarmColleague";
}

void AlarmColleague::onSecurityCommand(const std::string& command)
{
    if (command == CMD_ACTIVATE_ALARM) {
        activate();
    } else if (command == CMD_DEACTIVATE_ALARM) {
        deactivate();
    }
}

void AlarmColleague::setAlarm(Alarm* alarm)
{
    m_alarm = alarm;
}

Alarm* AlarmColleague::getAlarm() const
{
    return m_alarm;
}

void AlarmColleague::activate(const std::string& alarmType)
{
    if (m_alarm && !m_isActive) {
        AlarmType type = ALARM_INTRUSION;
        if (alarmType == "FIRE") {
            type = ALARM_FIRE;
        } else if (alarmType == "GAS") {
            type = ALARM_GAS_LEAK;
        } else if (alarmType == "INTRUSION" || alarmType == "SECURITY") {
            type = ALARM_INTRUSION;
        } else if (alarmType == "SYSTEM") {
            type = ALARM_SYSTEM_ERROR;
        }
        m_alarm->trigger(type);
        m_isActive = true;
        Logger::getInstance().warning("Alarm activated: " + alarmType);
        notifyMediator(EVENT_ALARM_TRIGGERED);
    }
}

void AlarmColleague::deactivate()
{
    if (m_alarm && m_isActive) {
        m_alarm->disarm("1234");
        m_isActive = false;
        Logger::getInstance().info("Alarm deactivated");
        notifyMediator(EVENT_ALARM_ACKNOWLEDGED);
    }
}

void AlarmColleague::silence()
{
    if (m_alarm) {
        m_alarm->silence();
        Logger::getInstance().info("Alarm silenced");
    }
}

bool AlarmColleague::isActive() const
{
    return m_isActive;
}
LightColleague::LightColleague()
    : m_isBlinking(false)
{
}

LightColleague::~LightColleague()
{
    m_lights.clear();
}

std::string LightColleague::getColleagueType() const
{
    return "LightColleague";
}

void LightColleague::onSecurityCommand(const std::string& command)
{
    if (command == CMD_TURN_ON_LIGHTS) {
        turnAllOn();
    } else if (command == CMD_TURN_OFF_LIGHTS) {
        turnAllOff();
    } else if (command == CMD_START_BLINKING) {
        startBlinking();
    } else if (command == CMD_STOP_BLINKING) {
        stopBlinking();
    }
}

void LightColleague::addLight(Light* light)
{
    if (light) {
        m_lights.push_back(light);
    }
}

void LightColleague::removeLight(Light* light)
{
    for (std::vector<Light*>::iterator it = m_lights.begin();
         it != m_lights.end(); ++it) {
        if (*it == light) {
            m_lights.erase(it);
            break;
        }
    }
}

void LightColleague::clearLights()
{
    m_lights.clear();
}

size_t LightColleague::getLightCount() const
{
    return m_lights.size();
}

void LightColleague::turnAllOn()
{
    for (size_t i = 0; i < m_lights.size(); ++i) {
        if (m_lights[i]) {
            m_lights[i]->turnOn();
        }
    }
    Logger::getInstance().info("All lights turned ON");
    notifyMediator(EVENT_LIGHTS_ON);
}

void LightColleague::turnAllOff()
{
    for (size_t i = 0; i < m_lights.size(); ++i) {
        if (m_lights[i]) {
            m_lights[i]->turnOff();
        }
    }
    Logger::getInstance().info("All lights turned OFF");
    notifyMediator(EVENT_LIGHTS_OFF);
}

void LightColleague::startBlinking()
{
    m_isBlinking = true;
    Logger::getInstance().warning("Lights blinking started");
    notifyMediator(EVENT_LIGHTS_BLINK);
}

void LightColleague::stopBlinking()
{
    m_isBlinking = false;
    Logger::getInstance().info("Lights blinking stopped");
}

void LightColleague::blinkOnce()
{
    performBlink();
}

bool LightColleague::isBlinking() const
{
    return m_isBlinking;
}

void LightColleague::performBlink()
{
    for (size_t i = 0; i < m_lights.size(); ++i) {
        if (m_lights[i]) {
            m_lights[i]->turnOff();
        }
    }
    for (size_t i = 0; i < m_lights.size(); ++i) {
        if (m_lights[i]) {
            m_lights[i]->turnOn();
        }
    }
}
CameraColleague::CameraColleague()
    : m_isRecording(false)
{
}

CameraColleague::~CameraColleague()
{
    m_cameras.clear();
}

std::string CameraColleague::getColleagueType() const
{
    return "CameraColleague";
}

void CameraColleague::onSecurityCommand(const std::string& command)
{
    if (command == CMD_START_RECORDING) {
        startRecordingAll();
    } else if (command == CMD_STOP_RECORDING) {
        stopRecordingAll();
    }
}

void CameraColleague::addCamera(Camera* camera)
{
    if (camera) {
        m_cameras.push_back(camera);
    }
}

void CameraColleague::removeCamera(Camera* camera)
{
    for (std::vector<Camera*>::iterator it = m_cameras.begin();
         it != m_cameras.end(); ++it) {
        if (*it == camera) {
            m_cameras.erase(it);
            break;
        }
    }
}

void CameraColleague::clearCameras()
{
    m_cameras.clear();
}

size_t CameraColleague::getCameraCount() const
{
    return m_cameras.size();
}

void CameraColleague::startRecordingAll()
{
    for (size_t i = 0; i < m_cameras.size(); ++i) {
        if (m_cameras[i]) {
            m_cameras[i]->startRecording();
        }
    }
    m_isRecording = true;
    Logger::getInstance().info("All cameras started recording");
    notifyMediator(EVENT_RECORDING_START);
}

void CameraColleague::stopRecordingAll()
{
    for (size_t i = 0; i < m_cameras.size(); ++i) {
        if (m_cameras[i]) {
            m_cameras[i]->stopRecording();
        }
    }
    m_isRecording = false;
    Logger::getInstance().info("All cameras stopped recording");
    notifyMediator(EVENT_RECORDING_STOP);
}

void CameraColleague::enableMotionDetectionAll()
{
    for (size_t i = 0; i < m_cameras.size(); ++i) {
        if (m_cameras[i]) {
            m_cameras[i]->enableMotionDetection(true);
        }
    }
    Logger::getInstance().info("Motion detection enabled on all cameras");
}

void CameraColleague::disableMotionDetectionAll()
{
    for (size_t i = 0; i < m_cameras.size(); ++i) {
        if (m_cameras[i]) {
            m_cameras[i]->enableMotionDetection(false);
        }
    }
    Logger::getInstance().info("Motion detection disabled on all cameras");
}

bool CameraColleague::isRecording() const
{
    return m_isRecording;
}

void CameraColleague::reportMotion()
{
    Logger::getInstance().warning("Motion detected by camera");
    notifyMediator(EVENT_MOTION_DETECTED);
}
DetectorColleague::DetectorColleague()
{
}

DetectorColleague::~DetectorColleague()
{
    m_detectors.clear();
}

std::string DetectorColleague::getColleagueType() const
{
    return "DetectorColleague";
}

void DetectorColleague::onSecurityCommand(const std::string& command)
{
    (void)command;
}

void DetectorColleague::addDetector(Detector* detector)
{
    if (detector) {
        m_detectors.push_back(detector);
    }
}

void DetectorColleague::removeDetector(Detector* detector)
{
    for (std::vector<Detector*>::iterator it = m_detectors.begin();
         it != m_detectors.end(); ++it) {
        if (*it == detector) {
            m_detectors.erase(it);
            break;
        }
    }
}

void DetectorColleague::clearDetectors()
{
    m_detectors.clear();
}

size_t DetectorColleague::getDetectorCount() const
{
    return m_detectors.size();
}

void DetectorColleague::reportSmokeDetected()
{
    Logger::getInstance().critical("Smoke detected!");
    notifyMediator(EVENT_SMOKE_DETECTED);
}

void DetectorColleague::reportGasDetected()
{
    Logger::getInstance().critical("Gas detected!");
    notifyMediator(EVENT_GAS_DETECTED);
}
EmergencyServiceColleague::EmergencyServiceColleague()
    : m_policeNumber("911")
    , m_fireStationNumber("911")
    , m_ambulanceNumber("911")
{
}

EmergencyServiceColleague::~EmergencyServiceColleague()
{
}

std::string EmergencyServiceColleague::getColleagueType() const
{
    return "EmergencyServiceColleague";
}

void EmergencyServiceColleague::onSecurityCommand(const std::string& command)
{
    if (command == CMD_CALL_POLICE) {
        callPolice();
    } else if (command == CMD_CALL_FIRE_STATION) {
        callFireStation();
    }
}

void EmergencyServiceColleague::callPolice()
{
    simulateCall("Police", m_policeNumber);
    notifyMediator(EVENT_CALL_POLICE);
}

void EmergencyServiceColleague::callFireStation()
{
    simulateCall("Fire Station", m_fireStationNumber);
    notifyMediator(EVENT_CALL_FIRE);
}

void EmergencyServiceColleague::callAmbulance()
{
    simulateCall("Ambulance", m_ambulanceNumber);
}

void EmergencyServiceColleague::setPoliceNumber(const std::string& number)
{
    m_policeNumber = number;
}

void EmergencyServiceColleague::setFireStationNumber(const std::string& number)
{
    m_fireStationNumber = number;
}

void EmergencyServiceColleague::setAmbulanceNumber(const std::string& number)
{
    m_ambulanceNumber = number;
}

std::string EmergencyServiceColleague::getPoliceNumber() const
{
    return m_policeNumber;
}

std::string EmergencyServiceColleague::getFireStationNumber() const
{
    return m_fireStationNumber;
}

std::string EmergencyServiceColleague::getAmbulanceNumber() const
{
    return m_ambulanceNumber;
}

void EmergencyServiceColleague::simulateCall(const std::string& service,
                                             const std::string& number)
{
    std::cout << "[EMERGENCY CALL] Calling " << service
              << " at " << number << "..." << std::endl;
    Logger::getInstance().critical("Emergency call made to " + service +
                                   " at " + number);
}

}
