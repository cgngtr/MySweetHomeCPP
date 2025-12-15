#include "Detector.h"
#include <sstream>

namespace MySweetHome {
Detector::Detector(uint32_t id, const std::string& name, DetectorType detectorType, const std::string& location)
    : Device(id, name, DEVICE_SMOKE_DETECTOR, location)
    , m_detectorType(detectorType)
    , m_alarmTriggered(false)
    , m_sensorValue(0.0f)
    , m_threshold(50.0f)
{
}

Detector::~Detector()
{
}

void Detector::testAlarm() {
    if (isOn()) {
        m_alarmTriggered = true;
    }
}

void Detector::resetAlarm() {
    m_alarmTriggered = false;
}

bool Detector::isAlarmTriggered() const {
    return m_alarmTriggered;
}

float Detector::getSensorValue() const {
    return m_sensorValue;
}

void Detector::setSensorValue(float value) {
    m_sensorValue = value;
    if (isOn() && m_sensorValue >= m_threshold) {
        triggerAlarm();
    }
}

void Detector::setThreshold(float threshold) {
    m_threshold = threshold;
}

float Detector::getThreshold() const {
    return m_threshold;
}

DetectorType Detector::getDetectorType() const {
    return m_detectorType;
}

void Detector::triggerAlarm() {
    m_alarmTriggered = true;
}

void Detector::turnOn() {
    Device::turnOn();
    m_alarmTriggered = false;
}

void Detector::turnOff() {
    m_alarmTriggered = false;
}

std::string Detector::getInfo() const {
    std::ostringstream oss;
    oss << Device::getInfo() << " | Type: ";

    switch (m_detectorType) {
        case DETECTOR_SMOKE:
            oss << "Smoke";
            break;
        case DETECTOR_GAS:
            oss << "Gas";
            break;
        case DETECTOR_CARBON_MONOXIDE:
            oss << "Carbon Monoxide";
            break;
        case DETECTOR_WATER_LEAK:
            oss << "Water Leak";
            break;
    }

    oss << " | Sensor: " << m_sensorValue << "/" << m_threshold
        << " | Alarm: " << (m_alarmTriggered ? "ACTIVE!" : "Normal");
    return oss.str();
}

bool Detector::isCritical() const {
    return true;
}
SmokeDetector::SmokeDetector(uint32_t id, const std::string& name, const std::string& location)
    : Detector(id, name, DETECTOR_SMOKE, location)
{
    setThreshold(30.0f);
}

SmokeDetector::~SmokeDetector()
{
}

Device* SmokeDetector::clone() const {
    SmokeDetector* newDetector = new SmokeDetector(getId(), getName(), getLocation());
    newDetector->setThreshold(m_threshold);
    return newDetector;
}

bool SmokeDetector::detectSmoke() {
    return isOn() && m_sensorValue >= m_threshold;
}
GasDetector::GasDetector(uint32_t id, const std::string& name, const std::string& location)
    : Detector(id, name, DETECTOR_GAS, location)
{
    setThreshold(100.0f);
}

GasDetector::~GasDetector()
{
}

Device* GasDetector::clone() const {
    GasDetector* newDetector = new GasDetector(getId(), getName(), getLocation());
    newDetector->setThreshold(m_threshold);
    return newDetector;
}

bool GasDetector::detectGas() {
    return isOn() && m_sensorValue >= m_threshold;
}

}
