#ifndef STATE_MANAGER_H
#define STATE_MANAGER_H

#include <string>
#include "common_types.h"
#include "StateMemento.h"
#include "ISystemState.h"

namespace MySweetHome {

class SmartHome;
class StateManager {
public:
    StateManager();
    ~StateManager();
    void setSmartHome(SmartHome* home);
    void setState(SystemState state);
    SystemState getCurrentState() const;
    std::string getCurrentStateString() const;
    ISystemState* getCurrentStateObject() const;
    bool goToPreviousState();
    bool goToNextState();
    bool canGoToPrevious() const;
    bool canGoToNext() const;
    int getHistorySize() const;

private:
    SystemState m_currentStateEnum;
    ISystemState* m_currentStateObject;
    StateHistory m_stateHistory;
    SmartHome* m_smartHome;
};

}

#endif
