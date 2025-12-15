#include "StateManager.h"
#include "SmartHome.h"
#include "Logger.h"

namespace MySweetHome {

StateManager::StateManager()
    : m_currentStateEnum(STATE_NORMAL)
    , m_currentStateObject(0)
    , m_smartHome(0)
{
    m_currentStateObject = StateFactory::createState(STATE_NORMAL);
    m_stateHistory.push(STATE_NORMAL);
}

StateManager::~StateManager()
{
    StateFactory::destroyState(m_currentStateObject);
}

void StateManager::setSmartHome(SmartHome* home) {
    m_smartHome = home;
}

void StateManager::setState(SystemState state) {
    if (state == m_currentStateEnum) {
        return;
    }
    if (m_currentStateObject) {
        m_currentStateObject->exit(m_smartHome);
        StateFactory::destroyState(m_currentStateObject);
    }
    m_stateHistory.push(state);
    m_currentStateEnum = state;
    m_currentStateObject = StateFactory::createState(state);
    if (m_currentStateObject) {
        m_currentStateObject->enter(m_smartHome);
    }

    Logger::getInstance().info("Durum degistirildi: " + getCurrentStateString());
}

SystemState StateManager::getCurrentState() const {
    return m_currentStateEnum;
}

std::string StateManager::getCurrentStateString() const {
    if (m_currentStateObject) {
        return m_currentStateObject->getName();
    }
    switch (m_currentStateEnum) {
        case STATE_NORMAL:           return "Normal";
        case STATE_HIGH_PERFORMANCE: return "Yuksek Performans";
        case STATE_LOW_POWER:        return "Dusuk Guc";
        case STATE_SLEEP:            return "Uyku";
        default:                     return "Bilinmiyor";
    }
}

ISystemState* StateManager::getCurrentStateObject() const {
    return m_currentStateObject;
}

bool StateManager::goToPreviousState() {
    if (!canGoToPrevious()) {
        return false;
    }

    StateMemento* memento = m_stateHistory.undo();
    if (memento) {
        if (m_currentStateObject) {
            m_currentStateObject->exit(m_smartHome);
            StateFactory::destroyState(m_currentStateObject);
        }

        m_currentStateEnum = memento->getState();
        m_currentStateObject = StateFactory::createState(m_currentStateEnum);
        if (m_currentStateObject) {
            m_currentStateObject->enter(m_smartHome);
        }

        Logger::getInstance().info("Onceki duruma donuldu: " + getCurrentStateString());
        return true;
    }
    return false;
}

bool StateManager::goToNextState() {
    if (!canGoToNext()) {
        return false;
    }

    StateMemento* memento = m_stateHistory.redo();
    if (memento) {
        if (m_currentStateObject) {
            m_currentStateObject->exit(m_smartHome);
            StateFactory::destroyState(m_currentStateObject);
        }

        m_currentStateEnum = memento->getState();
        m_currentStateObject = StateFactory::createState(m_currentStateEnum);
        if (m_currentStateObject) {
            m_currentStateObject->enter(m_smartHome);
        }

        Logger::getInstance().info("Sonraki duruma gecildi: " + getCurrentStateString());
        return true;
    }
    return false;
}

bool StateManager::canGoToPrevious() const {
    return m_stateHistory.canUndo();
}

bool StateManager::canGoToNext() const {
    return m_stateHistory.canRedo();
}

int StateManager::getHistorySize() const {
    return m_stateHistory.getHistorySize();
}

}
