#include "Logger.h"
#include <iostream>
#include <ctime>
#include <sstream>
#include <iomanip>

namespace MySweetHome {

Logger& Logger::getInstance() {
    static Logger instance;
    return instance;
}

Logger::Logger()
    : m_minLevel(LOG_INFO)
    , m_logToFile(false)
    , m_logToConsole(true)
    , m_logFilename("mysweethome.log")
{
}

Logger::~Logger() {
    closeLogFile();
}

void Logger::log(LogLevel level, const std::string& message) {
    if (level < m_minLevel) {
        return;
    }

    std::string formattedMessage = formatLogMessage(level, message);
    m_logBuffer.push_back(formattedMessage);
    if (m_logBuffer.size() > MAX_LOG_ENTRIES) {
        m_logBuffer.erase(m_logBuffer.begin());
    }
    if (m_logToConsole) {
        std::cout << formattedMessage << std::endl;
    }
    if (m_logToFile && m_logFile.is_open()) {
        m_logFile << formattedMessage << std::endl;
        m_logFile.flush();
    }
}

void Logger::debug(const std::string& message) {
    log(LOG_DEBUG, message);
}

void Logger::info(const std::string& message) {
    log(LOG_INFO, message);
}

void Logger::warning(const std::string& message) {
    log(LOG_WARNING, message);
}

void Logger::error(const std::string& message) {
    log(LOG_ERROR, message);
}

void Logger::critical(const std::string& message) {
    log(LOG_CRITICAL, message);
}

void Logger::setLogLevel(LogLevel level) {
    m_minLevel = level;
}

LogLevel Logger::getLogLevel() const {
    return m_minLevel;
}

void Logger::setLogToFile(bool enable) {
    m_logToFile = enable;

    if (enable && !m_logFile.is_open()) {
        openLogFile();
    } else if (!enable && m_logFile.is_open()) {
        closeLogFile();
    }
}

void Logger::setLogToConsole(bool enable) {
    m_logToConsole = enable;
}

void Logger::setLogFile(const std::string& filename) {
    if (m_logFile.is_open()) {
        closeLogFile();
    }

    m_logFilename = filename;

    if (m_logToFile) {
        openLogFile();
    }
}

void Logger::openLogFile() {
    if (!m_logFile.is_open()) {
        m_logFile.open(m_logFilename.c_str(), std::ios::app);
        if (m_logFile.is_open()) {
            info("Log file opened: " + m_logFilename);
        }
    }
}

void Logger::closeLogFile() {
    if (m_logFile.is_open()) {
        info("Log file closing: " + m_logFilename);
        m_logFile.close();
    }
}

std::vector<std::string> Logger::getRecentLogs(size_t count) const {
    if (count >= m_logBuffer.size()) {
        return m_logBuffer;
    }

    std::vector<std::string> result;
    size_t startIndex = m_logBuffer.size() - count;
    for (size_t i = startIndex; i < m_logBuffer.size(); ++i) {
        result.push_back(m_logBuffer[i]);
    }
    return result;
}

void Logger::clearLogs() {
    m_logBuffer.clear();
}

std::string Logger::formatLogMessage(LogLevel level, const std::string& message) {
    std::ostringstream oss;
    oss << "[" << getCurrentTimestamp() << "]"
        << " [" << logLevelToString(level) << "]"
        << " " << message;
    return oss.str();
}

std::string Logger::logLevelToString(LogLevel level) const {
    switch (level) {
        case LOG_DEBUG:    return "DEBUG";
        case LOG_INFO:     return "INFO ";
        case LOG_WARNING:  return "WARN ";
        case LOG_ERROR:    return "ERROR";
        case LOG_CRITICAL: return "CRIT ";
        default:           return "?????";
    }
}

std::string Logger::getCurrentTimestamp() const {
    time_t now = time(0);
    struct tm* timeinfo = localtime(&now);

    std::ostringstream oss;
    oss << (1900 + timeinfo->tm_year) << "-"
        << std::setfill('0') << std::setw(2) << (1 + timeinfo->tm_mon) << "-"
        << std::setfill('0') << std::setw(2) << timeinfo->tm_mday << " "
        << std::setfill('0') << std::setw(2) << timeinfo->tm_hour << ":"
        << std::setfill('0') << std::setw(2) << timeinfo->tm_min << ":"
        << std::setfill('0') << std::setw(2) << timeinfo->tm_sec;
    return oss.str();
}

}
