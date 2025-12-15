#ifndef SECURITY_COLLEAGUE_H
#define SECURITY_COLLEAGUE_H

#include "ISecurityMediator.h"
#include <vector>
#include <string>

namespace MySweetHome {
class Alarm;
class Light;
class Camera;
class Detector;
class BaseSecurityColleague : public ISecurityColleague {
public:
    BaseSecurityColleague();
    virtual ~BaseSecurityColleague();

    virtual void setMediator(ISecurityMediator* mediator);

protected:
    ISecurityMediator* m_mediator;
    void notifyMediator(const std::string& event);
};
class AlarmColleague : public BaseSecurityColleague {
public:
    AlarmColleague(Alarm* alarm = 0);
    virtual ~AlarmColleague();
    virtual std::string getColleagueType() const;
    virtual void onSecurityCommand(const std::string& command);
    void setAlarm(Alarm* alarm);
    Alarm* getAlarm() const;
    void activate(const std::string& alarmType = "SECURITY");
    void deactivate();
    void silence();
    bool isActive() const;

private:
    Alarm* m_alarm;
    bool m_isActive;
};
class LightColleague : public BaseSecurityColleague {
public:
    LightColleague();
    virtual ~LightColleague();
    virtual std::string getColleagueType() const;
    virtual void onSecurityCommand(const std::string& command);
    void addLight(Light* light);
    void removeLight(Light* light);
    void clearLights();
    size_t getLightCount() const;
    void turnAllOn();
    void turnAllOff();
    void startBlinking();
    void stopBlinking();
    void blinkOnce();
    bool isBlinking() const;

private:
    std::vector<Light*> m_lights;
    bool m_isBlinking;

    void performBlink();
};
class CameraColleague : public BaseSecurityColleague {
public:
    CameraColleague();
    virtual ~CameraColleague();
    virtual std::string getColleagueType() const;
    virtual void onSecurityCommand(const std::string& command);
    void addCamera(Camera* camera);
    void removeCamera(Camera* camera);
    void clearCameras();
    size_t getCameraCount() const;
    void startRecordingAll();
    void stopRecordingAll();
    void enableMotionDetectionAll();
    void disableMotionDetectionAll();
    bool isRecording() const;
    void reportMotion();

private:
    std::vector<Camera*> m_cameras;
    bool m_isRecording;
};
class DetectorColleague : public BaseSecurityColleague {
public:
    DetectorColleague();
    virtual ~DetectorColleague();
    virtual std::string getColleagueType() const;
    virtual void onSecurityCommand(const std::string& command);
    void addDetector(Detector* detector);
    void removeDetector(Detector* detector);
    void clearDetectors();
    size_t getDetectorCount() const;
    void reportSmokeDetected();
    void reportGasDetected();

private:
    std::vector<Detector*> m_detectors;
};
class EmergencyServiceColleague : public BaseSecurityColleague {
public:
    EmergencyServiceColleague();
    virtual ~EmergencyServiceColleague();
    virtual std::string getColleagueType() const;
    virtual void onSecurityCommand(const std::string& command);
    void callPolice();
    void callFireStation();
    void callAmbulance();
    void setPoliceNumber(const std::string& number);
    void setFireStationNumber(const std::string& number);
    void setAmbulanceNumber(const std::string& number);

    std::string getPoliceNumber() const;
    std::string getFireStationNumber() const;
    std::string getAmbulanceNumber() const;

private:
    std::string m_policeNumber;
    std::string m_fireStationNumber;
    std::string m_ambulanceNumber;

    void simulateCall(const std::string& service, const std::string& number);
};

}

#endif
