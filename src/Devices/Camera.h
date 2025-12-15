#ifndef CAMERA_H
#define CAMERA_H

#include "Device.h"

namespace MySweetHome {

enum CameraMode {
    CAMERA_IDLE,
    CAMERA_RECORDING,
    CAMERA_STREAMING,
    CAMERA_MOTION_DETECTION
};

class Camera : public Device {
public:
    Camera(uint32_t id, const std::string& name, const std::string& location);
    virtual ~Camera();
    void startRecording();
    void stopRecording();
    void startStreaming();
    void stopStreaming();
    void enableMotionDetection(bool enable);
    void setFPS(int fps);
    int getFPS() const;
    void setNightVision(bool enable);
    bool hasNightVision() const;
    CameraMode getCameraMode() const;
    bool isMotionDetectionEnabled() const;
    bool isRecording() const;
    bool detectMotion();
    virtual Device* clone() const;
    virtual void turnOn();
    virtual void turnOff();
    virtual std::string getInfo() const;
    virtual bool isCritical() const;

protected:
    CameraMode m_cameraMode;
    bool m_motionDetection;
    int m_fps;
    bool m_nightVision;
};
class SamsungCamera : public Camera {
public:
    SamsungCamera(uint32_t id, const std::string& location);
    virtual ~SamsungCamera();

    virtual Device* clone() const;
    virtual std::string getInfo() const;
    void enableSmartThings(bool enable);
    bool hasSmartThings() const;
    void setResolution(const std::string& resolution);
    std::string getResolution() const;

private:
    bool m_smartThingsEnabled;
    std::string m_resolution;
};
class LogitechCamera : public Camera {
public:
    LogitechCamera(uint32_t id, const std::string& location);
    virtual ~LogitechCamera();

    virtual Device* clone() const;
    virtual std::string getInfo() const;
    void enableAutoFocus(bool enable);
    bool hasAutoFocus() const;
    void setFieldOfView(int degrees);
    int getFieldOfView() const;

private:
    bool m_autoFocus;
    int m_fieldOfView;
};
class SonyCamera : public Camera {
public:
    SonyCamera(uint32_t id, const std::string& location);
    virtual ~SonyCamera();

    virtual Device* clone() const;
    virtual std::string getInfo() const;
    void enableStabilization(bool enable);
    bool hasStabilization() const;

private:
    bool m_stabilization;
};

}

#endif
