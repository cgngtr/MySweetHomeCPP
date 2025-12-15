#ifndef COMMAND_INVOKER_H
#define COMMAND_INVOKER_H

#include "ICommand.h"
#include <map>
#include <vector>
#include <string>

namespace MySweetHome {
class CommandInvoker {
public:
    CommandInvoker();
    ~CommandInvoker();
    void registerCommand(int key, ICommand* command);
    void unregisterCommand(int key);
    void clearCommands();
    bool executeCommand(int key);
    bool hasCommand(int key) const;
    void undoLastCommand();
    bool canUndo() const;
    size_t getHistorySize() const;
    void clearHistory();
    std::vector<int> getRegisteredKeys() const;
    ICommand* getCommand(int key) const;
    std::string getCommandName(int key) const;
    std::string getCommandDescription(int key) const;
    std::vector<std::string> getCommandList() const;

private:
    std::map<int, ICommand*> m_commands;
    std::vector<ICommand*> m_history;
    bool m_ownsCommands;
};

}

#endif
