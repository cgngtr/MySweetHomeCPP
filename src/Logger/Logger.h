#ifndef LOGGER_H
#define LOGGER_H

#include <string>
#include <vector>
#include <fstream>
#include "common_types.h"

namespace MySweetHome {

class Logger {
public:
    static Logger& getInstance();
    void log(LogLevel level, const std::string& message);
    void debug(const std::string& message);
    void info(const std::string& message);
    void warning(const std::string& message);
    void error(const std::string& message);
    void critical(const std::string& message);
    void setLogLevel(LogLevel level);
    LogLevel getLogLevel() const;

    void setLogToFile(bool enable);
    void setLogToConsole(bool enable);
    void setLogFile(const std::string& filename);
    void openLogFile();
    void closeLogFile();
    std::vector<std::string> getRecentLogs(size_t count) const;
    void clearLogs();

private:
    Logger();
    ~Logger();
    Logger(const Logger&);
    Logger& operator=(const Logger&);

    std::string formatLogMessage(LogLevel level, const std::string& message);
    std::string logLevelToString(LogLevel level) const;
    std::string getCurrentTimestamp() const;

    LogLevel m_minLevel;
    bool m_logToFile;
    bool m_logToConsole;
    std::string m_logFilename;
    std::ofstream m_logFile;
    std::vector<std::string> m_logBuffer;
};

}

#endif
