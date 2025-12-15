#include "SecurityManager.h"
#include "SmartHome.h"
#include "Alarm.h"
#include "Light.h"
#include "Camera.h"
#include "Detector.h"
#include "Logger.h"
#include <iostream>
#include <ctime>

#ifdef _WIN32
#include <windows.h>
#include <conio.h>
#else
#include <unistd.h>
#include <sys/select.h>
#include <termios.h>
#endif

namespace MySweetHome {

SecurityManager::SecurityManager(SmartHome* smartHome)
    : m_smartHome(smartHome)
    , m_alarm(0)
    , m_alarmAcknowledged(false)
    , m_sequenceActive(false)
    , m_isBlinking(false)
{
    std::vector<Device*> alarms = m_smartHome->getDevicesByType(DEVICE_ALARM);
    if (!alarms.empty()) {
        m_alarm = static_cast<Alarm*>(alarms[0]);
    }
}

SecurityManager::~SecurityManager()
{
}
void SecurityManager::handleMotionDetected() {
    if (m_sequenceActive) return;

    m_sequenceActive = true;
    m_alarmAcknowledged = false;

    Logger::getInstance().warning("GUVENLIK: Hareket algilandi!");
    std::cout << std::endl;
    std::cout << "  *** GUVENLIK ALARMI: Hareket Algilandi! ***" << std::endl;
    std::cout << std::endl;
    std::cout << "  [1/3] Alarm aktive edildi..." << std::endl;
    activateAlarm(ALARM_INTRUSION);

    for (int i = 5; i > 0; --i) {
        std::cout << "        Alarm aktif: " << i << " saniye kaldi" << std::endl;
        sleepSeconds(1);
        if (checkForKeyPress()) {
            acknowledgeAlarm();
            break;
        }
    }
    std::cout << "  [2/3] Tum isiklar aciliyor..." << std::endl;
    turnOnAllLights();

    for (int i = 5; i > 0; --i) {
        std::cout << "        Isiklar acik: " << i << " saniye kaldi" << std::endl;
        sleepSeconds(1);
        if (checkForKeyPress()) {
            acknowledgeAlarm();
            break;
        }
    }
    std::cout << "  [3/3] Polis araniyor..." << std::endl;
    callPolice();
    deactivateAlarm();
    m_sequenceActive = false;

    std::cout << std::endl;
    std::cout << "  Guvenlik dizisi tamamlandi." << std::endl;
    std::cout << std::endl;
}
void SecurityManager::handleSmokeDetected() {
    handleFireGasSequence(ALARM_FIRE, "Duman");
}
void SecurityManager::handleGasDetected() {
    handleFireGasSequence(ALARM_GAS_LEAK, "Gaz");
}
void SecurityManager::handleFireGasSequence(AlarmType type, const std::string& detectorName) {
    if (m_sequenceActive) return;

    m_sequenceActive = true;
    m_alarmAcknowledged = false;

    Logger::getInstance().warning("ALGILAMA: " + detectorName + " algilandi!");
    std::cout << std::endl;
    std::cout << "  *** ALGILAMA ALARMI: " << detectorName << " Algilandi! ***" << std::endl;
    std::cout << "  Alarmi onaylamak icin bir tusa basin." << std::endl;
    std::cout << std::endl;
    std::cout << "  [1/3] Alarm 10 saniye boyunca aktive edildi..." << std::endl;
    activateAlarm(type);

    bool acknowledged = false;
    for (int i = 10; i > 0 && !acknowledged; --i) {
        std::cout << "        Alarm aktif: " << i << " saniye kaldi (onaylamak icin tusa basin)" << std::endl;
        sleepSeconds(1);
        if (checkForKeyPress()) {
            acknowledgeAlarm();
            acknowledged = true;
        }
    }

    if (m_alarmAcknowledged) {
        std::cout << "  Alarm kullanici tarafindan onaylandi." << std::endl;
        deactivateAlarm();
        m_sequenceActive = false;
        return;
    }
    std::cout << "  [2/3] Kullanici onaylamadi. Isik yanip sonmeye basliyor..." << std::endl;
    std::cout << "  Isiklar 1 saniye aralikla yanip sonuyor. Onaylamak icin tusa basin." << std::endl;

    m_isBlinking = true;
    int blinkCount = 0;
    const int maxBlinks = 10;

    while (m_isBlinking && blinkCount < maxBlinks && !m_alarmAcknowledged) {
        blinkLightsOnce();
        sleepSeconds(1);
        blinkCount++;

        if (checkForKeyPress()) {
            acknowledgeAlarm();
            m_isBlinking = false;
        }
    }

    stopLightBlinking();

    if (m_alarmAcknowledged) {
        std::cout << "  Alarm yanip sonme sirasinda kullanici tarafindan onaylandi." << std::endl;
        deactivateAlarm();
        m_sequenceActive = false;
        return;
    }
    std::cout << "  [3/3] Kullanici onaylamadi. Itfaiye araniyor..." << std::endl;
    callFireStation();
    deactivateAlarm();
    turnOnAllLights();
    m_sequenceActive = false;

    std::cout << std::endl;
    std::cout << "  Algilama dizisi tamamlandi." << std::endl;
    std::cout << std::endl;
}

bool SecurityManager::waitForAcknowledgment(int timeoutSeconds) {
    time_t startTime = time(0);

    while (difftime(time(0), startTime) < timeoutSeconds) {
        if (checkForKeyPress()) {
            acknowledgeAlarm();
            return true;
        }
        sleepMilliseconds(100);
    }

    return false;
}

void SecurityManager::acknowledgeAlarm() {
    m_alarmAcknowledged = true;
    m_isBlinking = false;
    Logger::getInstance().info("Alarm kullanici tarafindan onaylandi.");
}

bool SecurityManager::isAlarmAcknowledged() const {
    return m_alarmAcknowledged;
}

void SecurityManager::callPolice() {
    Logger::getInstance().critical("ACIL DURUM: Polise cagri yapiliyor.");
    std::cout << std::endl;
    std::cout << "  ================================================" << std::endl;
    std::cout << "  =          Polise cagri yapiliyor.             =" << std::endl;
    std::cout << "  ================================================" << std::endl;
    std::cout << std::endl;
}

void SecurityManager::callFireStation() {
    Logger::getInstance().critical("ACIL DURUM: Itfaiyeye cagri yapiliyor.");
    std::cout << std::endl;
    std::cout << "  ================================================" << std::endl;
    std::cout << "  =        Itfaiyeye cagri yapiliyor.            =" << std::endl;
    std::cout << "  ================================================" << std::endl;
    std::cout << std::endl;
}

void SecurityManager::startLightBlinking() {
    m_isBlinking = true;
}

void SecurityManager::stopLightBlinking() {
    m_isBlinking = false;
    turnOnAllLights();
}

void SecurityManager::blinkLightsOnce() {
    std::vector<Device*> lights = m_smartHome->getDevicesByType(DEVICE_LIGHT);
    for (size_t i = 0; i < lights.size(); ++i) {
        lights[i]->toggle();
    }
}

bool SecurityManager::isSequenceActive() const {
    return m_sequenceActive;
}

void SecurityManager::resetSequence() {
    m_sequenceActive = false;
    m_alarmAcknowledged = false;
    m_isBlinking = false;
}

void SecurityManager::activateAlarm(AlarmType type) {
    if (m_alarm) {
        m_alarm->arm();
        m_alarm->trigger(type);
    }
}

void SecurityManager::deactivateAlarm() {
    if (m_alarm) {
        m_alarm->silence();
    }
}

void SecurityManager::turnOnAllLights() {
    std::vector<Device*> lights = m_smartHome->getDevicesByType(DEVICE_LIGHT);
    for (size_t i = 0; i < lights.size(); ++i) {
        lights[i]->turnOn();
    }
}

void SecurityManager::turnOffAllLights() {
    std::vector<Device*> lights = m_smartHome->getDevicesByType(DEVICE_LIGHT);
    for (size_t i = 0; i < lights.size(); ++i) {
        lights[i]->turnOff();
    }
}

void SecurityManager::sleepSeconds(int seconds) {
#ifdef _WIN32
    Sleep(seconds * 1000);
#else
    sleep(seconds);
#endif
}

void SecurityManager::sleepMilliseconds(int milliseconds) {
#ifdef _WIN32
    Sleep(milliseconds);
#else
    usleep(milliseconds * 1000);
#endif
}

bool SecurityManager::checkForKeyPress() {
#ifdef _WIN32
    if (_kbhit()) {
        _getch();
        return true;
    }
    return false;
#else
    fd_set readfds;
    struct timeval tv;

    FD_ZERO(&readfds);
    FD_SET(0, &readfds);

    tv.tv_sec = 0;
    tv.tv_usec = 0;

    if (select(1, &readfds, 0, 0, &tv) > 0) {
        char c;
        if (read(0, &c, 1) > 0) {
            return true;
        }
    }
    return false;
#endif
}

}
