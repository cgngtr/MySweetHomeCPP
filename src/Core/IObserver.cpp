#include "IObserver.h"
#include <iostream>

namespace MySweetHome {

NotificationManager::NotificationManager()
    : m_notificationType(NOTIFY_LOG)
{
}

NotificationManager::~NotificationManager()
{
}

void NotificationManager::onNotify(const std::string& event, const std::string& message) {
    switch (m_notificationType) {
        case NOTIFY_LOG:
            notifyByLog(message);
            break;
        case NOTIFY_ALARM:
            notifyByAlarm(message);
            break;
        case NOTIFY_SMS:
            notifyBySMS(message);
            break;
    }
}

void NotificationManager::setNotificationType(NotificationType type) {
    m_notificationType = type;
}

NotificationType NotificationManager::getNotificationType() const {
    return m_notificationType;
}

void NotificationManager::notifyByLog(const std::string& message) {
    std::cout << "[LOG] " << message << std::endl;
}

void NotificationManager::notifyByAlarm(const std::string& message) {
    std::cout << "[ALARM] !!! " << message << " !!!" << std::endl;
}

void NotificationManager::notifyBySMS(const std::string& message) {
    std::cout << "[SMS] A SMS is sent: " << message << std::endl;
}

}
