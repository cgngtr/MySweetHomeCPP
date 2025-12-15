#include "NotificationHandler.h"
#include "Logger.h"
#include <iostream>
#include <sstream>

namespace MySweetHome {
std::string severityToString(NotificationSeverity severity)
{
    switch (severity) {
        case SEVERITY_DEBUG:    return "DEBUG";
        case SEVERITY_INFO:     return "INFO";
        case SEVERITY_WARNING:  return "WARNING";
        case SEVERITY_ERROR:    return "ERROR";
        case SEVERITY_CRITICAL: return "CRITICAL";
        default:                return "UNKNOWN";
    }
}
BaseNotificationHandler::BaseNotificationHandler()
    : m_nextHandler(0)
{
}

BaseNotificationHandler::~BaseNotificationHandler()
{
}

void BaseNotificationHandler::setNext(INotificationHandler* handler)
{
    m_nextHandler = handler;
}

void BaseNotificationHandler::handle(NotificationSeverity severity,
                                     const std::string& event,
                                     const std::string& message)
{
    if (canHandle(severity)) {
        doHandle(event, message);
    }
    if (m_nextHandler) {
        m_nextHandler->handle(severity, event, message);
    }
}
LogNotificationHandler::LogNotificationHandler()
{
}

LogNotificationHandler::~LogNotificationHandler()
{
}

std::string LogNotificationHandler::getHandlerName() const
{
    return "LogHandler";
}

bool LogNotificationHandler::canHandle(NotificationSeverity severity) const
{
    (void)severity;
    return true;
}

void LogNotificationHandler::doHandle(const std::string& event,
                                      const std::string& message)
{
    std::ostringstream oss;
    oss << "[Notification] " << event << ": " << message;
    Logger::getInstance().info(oss.str());
}
ConsoleNotificationHandler::ConsoleNotificationHandler()
{
}

ConsoleNotificationHandler::~ConsoleNotificationHandler()
{
}

std::string ConsoleNotificationHandler::getHandlerName() const
{
    return "ConsoleHandler";
}

bool ConsoleNotificationHandler::canHandle(NotificationSeverity severity) const
{
    return severity >= SEVERITY_INFO;
}

void ConsoleNotificationHandler::doHandle(const std::string& event,
                                          const std::string& message)
{
    std::cout << "[NOTIFICATION] " << event << ": " << message << std::endl;
}
AlarmNotificationHandler::AlarmNotificationHandler()
    : m_alarmDuration(5)
{
}

AlarmNotificationHandler::~AlarmNotificationHandler()
{
}

std::string AlarmNotificationHandler::getHandlerName() const
{
    return "AlarmHandler";
}

void AlarmNotificationHandler::setAlarmDuration(int seconds)
{
    if (seconds > 0) {
        m_alarmDuration = seconds;
    }
}

int AlarmNotificationHandler::getAlarmDuration() const
{
    return m_alarmDuration;
}

bool AlarmNotificationHandler::canHandle(NotificationSeverity severity) const
{
    return severity >= SEVERITY_WARNING;
}

void AlarmNotificationHandler::doHandle(const std::string& event,
                                        const std::string& message)
{
    std::ostringstream oss;
    oss << "[ALARM] " << event << ": " << message
        << " (Duration: " << m_alarmDuration << "s)";
    std::cout << "\a" << oss.str() << std::endl;
    Logger::getInstance().warning("Alarm triggered: " + message);
}
SMSNotificationHandler::SMSNotificationHandler(const std::string& phoneNumber)
    : m_phoneNumber(phoneNumber)
{
}

SMSNotificationHandler::~SMSNotificationHandler()
{
}

std::string SMSNotificationHandler::getHandlerName() const
{
    return "SMSHandler";
}

void SMSNotificationHandler::setPhoneNumber(const std::string& number)
{
    m_phoneNumber = number;
}

std::string SMSNotificationHandler::getPhoneNumber() const
{
    return m_phoneNumber;
}

bool SMSNotificationHandler::canHandle(NotificationSeverity severity) const
{
    return severity >= SEVERITY_ERROR;
}

void SMSNotificationHandler::doHandle(const std::string& event,
                                      const std::string& message)
{
    std::ostringstream oss;
    oss << event << ": " << message;
    sendSMS(oss.str());
}

void SMSNotificationHandler::sendSMS(const std::string& message) const
{
    std::ostringstream oss;
    oss << "[SMS -> " << (m_phoneNumber.empty() ? "N/A" : m_phoneNumber)
        << "] " << message;
    std::cout << oss.str() << std::endl;
    Logger::getInstance().info("SMS notification sent: " + message);
}
EmailNotificationHandler::EmailNotificationHandler(const std::string& email)
    : m_email(email)
{
}

EmailNotificationHandler::~EmailNotificationHandler()
{
}

std::string EmailNotificationHandler::getHandlerName() const
{
    return "EmailHandler";
}

void EmailNotificationHandler::setEmailAddress(const std::string& email)
{
    m_email = email;
}

std::string EmailNotificationHandler::getEmailAddress() const
{
    return m_email;
}

bool EmailNotificationHandler::canHandle(NotificationSeverity severity) const
{
    return severity >= SEVERITY_ERROR;
}

void EmailNotificationHandler::doHandle(const std::string& event,
                                        const std::string& message)
{
    sendEmail("MySweetHome Alert: " + event, message);
}

void EmailNotificationHandler::sendEmail(const std::string& subject,
                                         const std::string& body) const
{
    std::ostringstream oss;
    oss << "[EMAIL -> " << (m_email.empty() ? "N/A" : m_email)
        << "] Subject: " << subject << ", Body: " << body;
    std::cout << oss.str() << std::endl;
    Logger::getInstance().info("Email notification sent: " + subject);
}
EmergencyNotificationHandler::EmergencyNotificationHandler()
{
}

EmergencyNotificationHandler::~EmergencyNotificationHandler()
{
}

std::string EmergencyNotificationHandler::getHandlerName() const
{
    return "EmergencyHandler";
}

bool EmergencyNotificationHandler::canHandle(NotificationSeverity severity) const
{
    return severity == SEVERITY_CRITICAL;
}

void EmergencyNotificationHandler::doHandle(const std::string& event,
                                            const std::string& message)
{
    std::ostringstream oss;
    oss << event << " - " << message;
    callEmergencyServices(oss.str());
}

void EmergencyNotificationHandler::callEmergencyServices(const std::string& message) const
{
    std::cout << "[EMERGENCY CALL] Contacting emergency services..." << std::endl;
    std::cout << "[EMERGENCY CALL] Reason: " << message << std::endl;
    Logger::getInstance().critical("Emergency services contacted: " + message);
}
INotificationHandler* NotificationChainBuilder::buildDefaultChain()
{
    LogNotificationHandler* logHandler = new LogNotificationHandler();
    ConsoleNotificationHandler* consoleHandler = new ConsoleNotificationHandler();
    AlarmNotificationHandler* alarmHandler = new AlarmNotificationHandler();
    SMSNotificationHandler* smsHandler = new SMSNotificationHandler("+90555555555");
    EmergencyNotificationHandler* emergencyHandler = new EmergencyNotificationHandler();
    logHandler->setNext(consoleHandler);
    consoleHandler->setNext(alarmHandler);
    alarmHandler->setNext(smsHandler);
    smsHandler->setNext(emergencyHandler);

    return logHandler;
}

INotificationHandler* NotificationChainBuilder::buildChain(bool enableLog,
                                                           bool enableConsole,
                                                           bool enableAlarm,
                                                           bool enableSMS,
                                                           bool enableEmail,
                                                           bool enableEmergency)
{
    INotificationHandler* head = 0;
    INotificationHandler* tail = 0;
    #define ADD_HANDLER(handler) \
        if (!head) { head = handler; tail = handler; } \
        else { tail->setNext(handler); tail = handler; }

    if (enableLog) {
        ADD_HANDLER(new LogNotificationHandler());
    }

    if (enableConsole) {
        ADD_HANDLER(new ConsoleNotificationHandler());
    }

    if (enableAlarm) {
        ADD_HANDLER(new AlarmNotificationHandler());
    }

    if (enableSMS) {
        ADD_HANDLER(new SMSNotificationHandler());
    }

    if (enableEmail) {
        ADD_HANDLER(new EmailNotificationHandler());
    }

    if (enableEmergency) {
        ADD_HANDLER(new EmergencyNotificationHandler());
    }

    #undef ADD_HANDLER
    if (!head) {
        head = new LogNotificationHandler();
    }

    return head;
}

INotificationHandler* NotificationChainBuilder::buildSimpleChain()
{
    LogNotificationHandler* logHandler = new LogNotificationHandler();
    ConsoleNotificationHandler* consoleHandler = new ConsoleNotificationHandler();

    logHandler->setNext(consoleHandler);

    return logHandler;
}

INotificationHandler* NotificationChainBuilder::buildChainFromType(NotificationType type)
{
    switch (type) {
        case NOTIFY_LOG:
            return new LogNotificationHandler();

        case NOTIFY_ALARM: {
            LogNotificationHandler* logHandler = new LogNotificationHandler();
            AlarmNotificationHandler* alarmHandler = new AlarmNotificationHandler();
            logHandler->setNext(alarmHandler);
            return logHandler;
        }

        case NOTIFY_SMS: {
            LogNotificationHandler* logHandler = new LogNotificationHandler();
            SMSNotificationHandler* smsHandler = new SMSNotificationHandler();
            logHandler->setNext(smsHandler);
            return logHandler;
        }

        default:
            return new LogNotificationHandler();
    }
}

}
