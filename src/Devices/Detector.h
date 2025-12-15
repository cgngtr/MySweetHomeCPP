#ifndef DETECTOR_H
#define DETECTOR_H

#include "Device.h"

namespace MySweetHome {

enum DetectorType {
    DETECTOR_SMOKE,
    DETECTOR_GAS,
    DETECTOR_CARBON_MONOXIDE,
    DETECTOR_WATER_LEAK
};

class Detector : public Device {
public:
    Detector(uint32_t id, const std::string& name, DetectorType detectorType, const std::string& location);
    virtual ~Detector();
    void testAlarm();
    void resetAlarm();
    bool isAlarmTriggered() const;
    float getSensorValue() const;
    void setSensorValue(float value);
    void setThreshold(float threshold);
    float getThreshold() const;

    DetectorType getDetectorType() const;
    virtual Device* clone() const = 0;
    virtual void turnOn();
    virtual void turnOff();
    virtual std::string getInfo() const;
    virtual bool isCritical() const;

protected:
    void triggerAlarm();

    DetectorType m_detectorType;
    bool m_alarmTriggered;
    float m_sensorValue;
    float m_threshold;
};
class SmokeDetector : public Detector {
public:
    SmokeDetector(uint32_t id, const std::string& name, const std::string& location);
    virtual ~SmokeDetector();

    virtual Device* clone() const;
    bool detectSmoke();
};
class GasDetector : public Detector {
public:
    GasDetector(uint32_t id, const std::string& name, const std::string& location);
    virtual ~GasDetector();

    virtual Device* clone() const;
    bool detectGas();
};

}

#endif
