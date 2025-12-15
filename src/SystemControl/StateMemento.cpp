#include "StateMemento.h"

namespace MySweetHome {
StateMemento::StateMemento(SystemState state)
    : m_savedState(state)
{
}

StateMemento::~StateMemento()
{
}

SystemState StateMemento::getState() const {
    return m_savedState;
}
StateHistory::StateHistory()
    : m_currentIndex(-1)
{
}

StateHistory::~StateHistory() {
    clear();
}

void StateHistory::push(SystemState state) {
    while (m_currentIndex < static_cast<int>(m_history.size()) - 1) {
        delete m_history.back();
        m_history.pop_back();
    }

    m_history.push_back(new StateMemento(state));
    m_currentIndex = static_cast<int>(m_history.size()) - 1;
}

bool StateHistory::canUndo() const {
    return m_currentIndex > 0;
}

StateMemento* StateHistory::undo() {
    if (!canUndo()) {
        return 0;
    }
    m_currentIndex--;
    return m_history[m_currentIndex];
}

bool StateHistory::canRedo() const {
    return m_currentIndex < static_cast<int>(m_history.size()) - 1;
}

StateMemento* StateHistory::redo() {
    if (!canRedo()) {
        return 0;
    }
    m_currentIndex++;
    return m_history[m_currentIndex];
}

int StateHistory::getCurrentIndex() const {
    return m_currentIndex;
}

int StateHistory::getHistorySize() const {
    return static_cast<int>(m_history.size());
}

void StateHistory::clear() {
    for (size_t i = 0; i < m_history.size(); ++i) {
        delete m_history[i];
    }
    m_history.clear();
    m_currentIndex = -1;
}

}
