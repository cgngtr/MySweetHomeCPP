#ifndef ISYSTEM_STATE_H
#define ISYSTEM_STATE_H

#include <string>
#include "common_types.h"

namespace MySweetHome {

class SmartHome;
class ISystemState {
public:
    virtual ~ISystemState() {}
    virtual std::string getName() const = 0;
    virtual SystemState getStateEnum() const = 0;
    virtual void enter(SmartHome* home) = 0;
    virtual void exit(SmartHome* home) = 0;
    virtual void handleDeviceOperation(SmartHome* home) = 0;
};
class NormalState : public ISystemState {
public:
    NormalState();
    virtual ~NormalState();

    virtual std::string getName() const;
    virtual SystemState getStateEnum() const;
    virtual void enter(SmartHome* home);
    virtual void exit(SmartHome* home);
    virtual void handleDeviceOperation(SmartHome* home);
};
class HighPerformanceState : public ISystemState {
public:
    HighPerformanceState();
    virtual ~HighPerformanceState();

    virtual std::string getName() const;
    virtual SystemState getStateEnum() const;
    virtual void enter(SmartHome* home);
    virtual void exit(SmartHome* home);
    virtual void handleDeviceOperation(SmartHome* home);
};
class LowPowerState : public ISystemState {
public:
    LowPowerState();
    virtual ~LowPowerState();

    virtual std::string getName() const;
    virtual SystemState getStateEnum() const;
    virtual void enter(SmartHome* home);
    virtual void exit(SmartHome* home);
    virtual void handleDeviceOperation(SmartHome* home);
};
class SleepState : public ISystemState {
public:
    SleepState();
    virtual ~SleepState();

    virtual std::string getName() const;
    virtual SystemState getStateEnum() const;
    virtual void enter(SmartHome* home);
    virtual void exit(SmartHome* home);
    virtual void handleDeviceOperation(SmartHome* home);
};
class StateFactory {
public:
    static ISystemState* createState(SystemState state);
    static void destroyState(ISystemState* state);
};

}

#endif
