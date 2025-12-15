#ifndef IOBSERVER_H
#define IOBSERVER_H

#include <string>
#include <vector>
#include "common_types.h"

namespace MySweetHome {
class IObserver {
public:
    virtual ~IObserver() {}
    virtual void onNotify(const std::string& event, const std::string& message) = 0;
};
class ISubject {
public:
    virtual ~ISubject() {}
    virtual void addObserver(IObserver* observer) = 0;
    virtual void removeObserver(IObserver* observer) = 0;
    virtual void notifyObservers(const std::string& event, const std::string& message) = 0;
};
class NotificationManager : public IObserver {
public:
    NotificationManager();
    virtual ~NotificationManager();

    void onNotify(const std::string& event, const std::string& message);

    void setNotificationType(NotificationType type);
    NotificationType getNotificationType() const;

    void notifyByLog(const std::string& message);
    void notifyByAlarm(const std::string& message);
    void notifyBySMS(const std::string& message);

private:
    NotificationType m_notificationType;
};

}

#endif
