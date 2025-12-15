#include "ModeManager.h"
#include "Device.h"
#include "Logger.h"

namespace MySweetHome {

ModeManager::ModeManager()
    : m_currentMode(MODE_NORMAL)
{
}

ModeManager::~ModeManager()
{
}

void ModeManager::setMode(SystemMode mode) {
    if (mode != m_currentMode) {
        m_currentMode = mode;
        Logger::getInstance().info("Sistem modu degistirildi: " + getCurrentModeString());
    }
}

SystemMode ModeManager::getCurrentMode() const {
    return m_currentMode;
}

std::string ModeManager::getCurrentModeString() const {
    switch (m_currentMode) {
        case MODE_NORMAL:  return "Normal";
        case MODE_EVENING: return "Aksam";
        case MODE_PARTY:   return "Parti";
        case MODE_CINEMA:  return "Sinema";
        default:           return "Bilinmiyor";
    }
}

bool ModeManager::shouldLightBeOn() const {
    switch (m_currentMode) {
        case MODE_NORMAL:  return true;
        case MODE_EVENING: return false;
        case MODE_PARTY:   return true;
        case MODE_CINEMA:  return false;
        default:           return true;
    }
}

bool ModeManager::shouldTVBeOn() const {
    switch (m_currentMode) {
        case MODE_NORMAL:  return false;
        case MODE_EVENING: return false;
        case MODE_PARTY:   return false;
        case MODE_CINEMA:  return true;
        default:           return false;
    }
}

bool ModeManager::shouldMusicBeOn() const {
    switch (m_currentMode) {
        case MODE_NORMAL:  return false;
        case MODE_EVENING: return false;
        case MODE_PARTY:   return true;
        case MODE_CINEMA:  return false;
        default:           return false;
    }
}

void ModeManager::applyModeToDevices(std::vector<Device*>& devices) {
    for (size_t i = 0; i < devices.size(); ++i) {
        Device* device = devices[i];
        if (!device) continue;
        if (device->isCritical()) {
            device->turnOn();
            continue;
        }

        switch (device->getType()) {
            case DEVICE_LIGHT:
                if (shouldLightBeOn()) {
                    device->turnOn();
                } else {
                    device->turnOff();
                }
                break;

            case DEVICE_TV:
                if (shouldTVBeOn()) {
                    device->turnOn();
                } else {
                    device->turnOff();
                }
                break;

            case DEVICE_SOUND_SYSTEM:
                if (shouldMusicBeOn()) {
                    device->turnOn();
                } else {
                    device->turnOff();
                }
                break;

            default:
                break;
        }
    }
}

}
