#ifndef NOTIFICATION_HANDLER_H
#define NOTIFICATION_HANDLER_H

#include <string>
#include "common_types.h"

namespace MySweetHome {
enum NotificationSeverity {
    SEVERITY_DEBUG = 0,
    SEVERITY_INFO = 1,
    SEVERITY_WARNING = 2,
    SEVERITY_ERROR = 3,
    SEVERITY_CRITICAL = 4
};
std::string severityToString(NotificationSeverity severity);
class INotificationHandler {
public:
    virtual ~INotificationHandler() {}
    virtual void setNext(INotificationHandler* handler) = 0;
    virtual void handle(NotificationSeverity severity,
                       const std::string& event,
                       const std::string& message) = 0;
    virtual std::string getHandlerName() const = 0;
};
class BaseNotificationHandler : public INotificationHandler {
public:
    BaseNotificationHandler();
    virtual ~BaseNotificationHandler();

    virtual void setNext(INotificationHandler* handler);
    virtual void handle(NotificationSeverity severity,
                       const std::string& event,
                       const std::string& message);

protected:
    virtual bool canHandle(NotificationSeverity severity) const = 0;
    virtual void doHandle(const std::string& event,
                         const std::string& message) = 0;

    INotificationHandler* m_nextHandler;
};
class LogNotificationHandler : public BaseNotificationHandler {
public:
    LogNotificationHandler();
    virtual ~LogNotificationHandler();

    virtual std::string getHandlerName() const;

protected:
    virtual bool canHandle(NotificationSeverity severity) const;
    virtual void doHandle(const std::string& event, const std::string& message);
};
class ConsoleNotificationHandler : public BaseNotificationHandler {
public:
    ConsoleNotificationHandler();
    virtual ~ConsoleNotificationHandler();

    virtual std::string getHandlerName() const;

protected:
    virtual bool canHandle(NotificationSeverity severity) const;
    virtual void doHandle(const std::string& event, const std::string& message);
};
class AlarmNotificationHandler : public BaseNotificationHandler {
public:
    AlarmNotificationHandler();
    virtual ~AlarmNotificationHandler();

    virtual std::string getHandlerName() const;
    void setAlarmDuration(int seconds);
    int getAlarmDuration() const;

protected:
    virtual bool canHandle(NotificationSeverity severity) const;
    virtual void doHandle(const std::string& event, const std::string& message);

private:
    int m_alarmDuration;
};
class SMSNotificationHandler : public BaseNotificationHandler {
public:
    SMSNotificationHandler(const std::string& phoneNumber = "");
    virtual ~SMSNotificationHandler();

    virtual std::string getHandlerName() const;
    void setPhoneNumber(const std::string& number);
    std::string getPhoneNumber() const;

protected:
    virtual bool canHandle(NotificationSeverity severity) const;
    virtual void doHandle(const std::string& event, const std::string& message);

private:
    std::string m_phoneNumber;
    void sendSMS(const std::string& message) const;
};
class EmailNotificationHandler : public BaseNotificationHandler {
public:
    EmailNotificationHandler(const std::string& email = "");
    virtual ~EmailNotificationHandler();

    virtual std::string getHandlerName() const;
    void setEmailAddress(const std::string& email);
    std::string getEmailAddress() const;

protected:
    virtual bool canHandle(NotificationSeverity severity) const;
    virtual void doHandle(const std::string& event, const std::string& message);

private:
    std::string m_email;
    void sendEmail(const std::string& subject, const std::string& body) const;
};
class EmergencyNotificationHandler : public BaseNotificationHandler {
public:
    EmergencyNotificationHandler();
    virtual ~EmergencyNotificationHandler();

    virtual std::string getHandlerName() const;

protected:
    virtual bool canHandle(NotificationSeverity severity) const;
    virtual void doHandle(const std::string& event, const std::string& message);

private:
    void callEmergencyServices(const std::string& message) const;
};
class NotificationChainBuilder {
public:
    static INotificationHandler* buildDefaultChain();
    static INotificationHandler* buildChain(bool enableLog,
                                            bool enableConsole,
                                            bool enableAlarm,
                                            bool enableSMS,
                                            bool enableEmail,
                                            bool enableEmergency);
    static INotificationHandler* buildSimpleChain();
    static INotificationHandler* buildChainFromType(NotificationType type);
};

}

#endif
