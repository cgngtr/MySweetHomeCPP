#ifndef STATE_MEMENTO_H
#define STATE_MEMENTO_H

#include <vector>
#include "common_types.h"

namespace MySweetHome {
class StateMemento {
public:
    StateMemento(SystemState state);
    ~StateMemento();

    SystemState getState() const;

private:
    SystemState m_savedState;
};
class StateHistory {
public:
    StateHistory();
    ~StateHistory();
    void push(SystemState state);
    bool canUndo() const;
    StateMemento* undo();
    bool canRedo() const;
    StateMemento* redo();
    int getCurrentIndex() const;
    int getHistorySize() const;
    void clear();

private:
    std::vector<StateMemento*> m_history;
    int m_currentIndex;
};

}

#endif
