#include "ISystemState.h"
#include "SmartHome.h"
#include "Logger.h"

namespace MySweetHome {
NormalState::NormalState() {}
NormalState::~NormalState() {}

std::string NormalState::getName() const {
    return "Normal";
}

SystemState NormalState::getStateEnum() const {
    return STATE_NORMAL;
}

void NormalState::enter(SmartHome* home) {
    Logger::getInstance().info("Normal duruma geciliyor - tum sistemler operasyonel.");
    if (home) {
    }
}

void NormalState::exit(SmartHome* home) {
    Logger::getInstance().info("Normal durumdan cikiliyor.");
}

void NormalState::handleDeviceOperation(SmartHome* home) {
}
HighPerformanceState::HighPerformanceState() {}
HighPerformanceState::~HighPerformanceState() {}

std::string HighPerformanceState::getName() const {
    return "Yuksek Performans";
}

SystemState HighPerformanceState::getStateEnum() const {
    return STATE_HIGH_PERFORMANCE;
}

void HighPerformanceState::enter(SmartHome* home) {
    Logger::getInstance().info("Yuksek Performans durumuna geciliyor - maksimum performans aktif.");
    if (home) {
        home->turnAllOn();
    }
}

void HighPerformanceState::exit(SmartHome* home) {
    Logger::getInstance().info("Yuksek Performans durumundan cikiliyor.");
}

void HighPerformanceState::handleDeviceOperation(SmartHome* home) {
}
LowPowerState::LowPowerState() {}
LowPowerState::~LowPowerState() {}

std::string LowPowerState::getName() const {
    return "Dusuk Guc";
}

SystemState LowPowerState::getStateEnum() const {
    return STATE_LOW_POWER;
}

void LowPowerState::enter(SmartHome* home) {
    Logger::getInstance().info("Dusuk Guc durumuna geciliyor - guc tuketimi azaltiliyor.");
    if (home) {
        home->turnOffByType(DEVICE_TV);
        home->turnOffByType(DEVICE_SOUND_SYSTEM);
    }
}

void LowPowerState::exit(SmartHome* home) {
    Logger::getInstance().info("Dusuk Guc durumundan cikiliyor.");
}

void LowPowerState::handleDeviceOperation(SmartHome* home) {
}
SleepState::SleepState() {}
SleepState::~SleepState() {}

std::string SleepState::getName() const {
    return "Uyku";
}

SystemState SleepState::getStateEnum() const {
    return STATE_SLEEP;
}

void SleepState::enter(SmartHome* home) {
    Logger::getInstance().info("Uyku durumuna geciliyor - minimal calisma modu.");
    if (home) {
        home->turnAllOff();
    }
}

void SleepState::exit(SmartHome* home) {
    Logger::getInstance().info("Uyku durumundan cikiliyor.");
}

void SleepState::handleDeviceOperation(SmartHome* home) {
}
ISystemState* StateFactory::createState(SystemState state) {
    switch (state) {
        case STATE_NORMAL:
            return new NormalState();
        case STATE_HIGH_PERFORMANCE:
            return new HighPerformanceState();
        case STATE_LOW_POWER:
            return new LowPowerState();
        case STATE_SLEEP:
            return new SleepState();
        default:
            return new NormalState();
    }
}

void StateFactory::destroyState(ISystemState* state) {
    delete state;
}

}
