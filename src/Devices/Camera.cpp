#include "Camera.h"
#include <sstream>

namespace MySweetHome {

Camera::Camera(uint32_t id, const std::string& name, const std::string& location)
    : Device(id, name, DEVICE_CAMERA, location)
    , m_cameraMode(CAMERA_IDLE)
    , m_motionDetection(false)
    , m_fps(24)
    , m_nightVision(false)
{
}

Camera::~Camera()
{
}

void Camera::startRecording() {
    if (isOn()) {
        m_cameraMode = CAMERA_RECORDING;
    }
}

void Camera::stopRecording() {
    if (m_cameraMode == CAMERA_RECORDING) {
        m_cameraMode = CAMERA_IDLE;
    }
}

void Camera::startStreaming() {
    if (isOn()) {
        m_cameraMode = CAMERA_STREAMING;
    }
}

void Camera::stopStreaming() {
    if (m_cameraMode == CAMERA_STREAMING) {
        m_cameraMode = CAMERA_IDLE;
    }
}

void Camera::enableMotionDetection(bool enable) {
    m_motionDetection = enable;
    if (enable && isOn()) {
        m_cameraMode = CAMERA_MOTION_DETECTION;
    }
}

void Camera::setFPS(int fps) {
    if (fps > 0 && fps <= 60) {
        m_fps = fps;
    }
}

int Camera::getFPS() const {
    return m_fps;
}

void Camera::setNightVision(bool enable) {
    m_nightVision = enable;
}

bool Camera::hasNightVision() const {
    return m_nightVision;
}

CameraMode Camera::getCameraMode() const {
    return m_cameraMode;
}

bool Camera::isMotionDetectionEnabled() const {
    return m_motionDetection;
}

bool Camera::isRecording() const {
    return m_cameraMode == CAMERA_RECORDING;
}

bool Camera::detectMotion() {
    return m_motionDetection && isOn();
}

Device* Camera::clone() const {
    Camera* newCamera = new Camera(getId(), getName(), getLocation());
    newCamera->setFPS(m_fps);
    newCamera->setNightVision(m_nightVision);
    newCamera->enableMotionDetection(m_motionDetection);
    return newCamera;
}

void Camera::turnOn() {
    Device::turnOn();
    m_cameraMode = CAMERA_IDLE;
}

void Camera::turnOff() {
    m_cameraMode = CAMERA_IDLE;
}

std::string Camera::getInfo() const {
    std::ostringstream oss;
    oss << Device::getInfo() << " | Mod: ";

    switch (m_cameraMode) {
        case CAMERA_IDLE:
            oss << "Beklemede";
            break;
        case CAMERA_RECORDING:
            oss << "Kayit Yapiyor";
            break;
        case CAMERA_STREAMING:
            oss << "Canli Yayin";
            break;
        case CAMERA_MOTION_DETECTION:
            oss << "Hareket Algilama";
            break;
    }

    oss << " | FPS: " << m_fps
        << " | Gece Gorusu: " << (m_nightVision ? "Evet" : "Hayir")
        << " | Hareket Algilama: " << (m_motionDetection ? "Aktif" : "Pasif");
    return oss.str();
}

bool Camera::isCritical() const {
    return true;
}
SamsungCamera::SamsungCamera(uint32_t id, const std::string& location)
    : Camera(id, "Samsung SmartCam", location)
    , m_smartThingsEnabled(true)
    , m_resolution("1080p")
{
}

SamsungCamera::~SamsungCamera()
{
}

Device* SamsungCamera::clone() const {
    SamsungCamera* newCamera = new SamsungCamera(getId(), getLocation());
    newCamera->setFPS(m_fps);
    newCamera->setNightVision(m_nightVision);
    newCamera->enableMotionDetection(m_motionDetection);
    newCamera->enableSmartThings(m_smartThingsEnabled);
    newCamera->setResolution(m_resolution);
    return newCamera;
}

std::string SamsungCamera::getInfo() const {
    std::ostringstream oss;
    oss << Camera::getInfo()
        << " | Marka: Samsung"
        << " | Cozunurluk: " << m_resolution
        << " | SmartThings: " << (m_smartThingsEnabled ? "Aktif" : "Pasif");
    return oss.str();
}

void SamsungCamera::enableSmartThings(bool enable) {
    m_smartThingsEnabled = enable;
}

bool SamsungCamera::hasSmartThings() const {
    return m_smartThingsEnabled;
}

void SamsungCamera::setResolution(const std::string& resolution) {
    m_resolution = resolution;
}

std::string SamsungCamera::getResolution() const {
    return m_resolution;
}
LogitechCamera::LogitechCamera(uint32_t id, const std::string& location)
    : Camera(id, "Logitech Webcam", location)
    , m_autoFocus(true)
    , m_fieldOfView(78)
{
}

LogitechCamera::~LogitechCamera()
{
}

Device* LogitechCamera::clone() const {
    LogitechCamera* newCamera = new LogitechCamera(getId(), getLocation());
    newCamera->setFPS(m_fps);
    newCamera->setNightVision(m_nightVision);
    newCamera->enableMotionDetection(m_motionDetection);
    newCamera->enableAutoFocus(m_autoFocus);
    newCamera->setFieldOfView(m_fieldOfView);
    return newCamera;
}

std::string LogitechCamera::getInfo() const {
    std::ostringstream oss;
    oss << Camera::getInfo()
        << " | Marka: Logitech"
        << " | Gorus Acisi: " << m_fieldOfView << " derece"
        << " | Otomatik Odak: " << (m_autoFocus ? "Aktif" : "Pasif");
    return oss.str();
}

void LogitechCamera::enableAutoFocus(bool enable) {
    m_autoFocus = enable;
}

bool LogitechCamera::hasAutoFocus() const {
    return m_autoFocus;
}

void LogitechCamera::setFieldOfView(int degrees) {
    if (degrees >= 60 && degrees <= 180) {
        m_fieldOfView = degrees;
    }
}

int LogitechCamera::getFieldOfView() const {
    return m_fieldOfView;
}
SonyCamera::SonyCamera(uint32_t id, const std::string& location)
    : Camera(id, "Sony Security Camera", location)
    , m_stabilization(true)
{
}

SonyCamera::~SonyCamera()
{
}

Device* SonyCamera::clone() const {
    SonyCamera* newCamera = new SonyCamera(getId(), getLocation());
    newCamera->setFPS(m_fps);
    newCamera->setNightVision(m_nightVision);
    newCamera->enableMotionDetection(m_motionDetection);
    newCamera->enableStabilization(m_stabilization);
    return newCamera;
}

std::string SonyCamera::getInfo() const {
    std::ostringstream oss;
    oss << Camera::getInfo()
        << " | Marka: Sony"
        << " | Stabilizasyon: " << (m_stabilization ? "Aktif" : "Pasif");
    return oss.str();
}

void SonyCamera::enableStabilization(bool enable) {
    m_stabilization = enable;
}

bool SonyCamera::hasStabilization() const {
    return m_stabilization;
}

}
