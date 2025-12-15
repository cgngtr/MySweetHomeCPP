#include "CommandInvoker.h"
#include <sstream>

namespace MySweetHome {
CommandInvoker::CommandInvoker()
    : m_ownsCommands(true)
{
}

CommandInvoker::~CommandInvoker()
{
    clearHistory();

    if (m_ownsCommands) {
        for (std::map<int, ICommand*>::iterator it = m_commands.begin();
             it != m_commands.end(); ++it) {
            delete it->second;
        }
    }
    m_commands.clear();
}
void CommandInvoker::registerCommand(int key, ICommand* command)
{
    if (command) {
        if (m_commands.find(key) != m_commands.end()) {
            if (m_ownsCommands) {
                delete m_commands[key];
            }
        }
        m_commands[key] = command;
    }
}

void CommandInvoker::unregisterCommand(int key)
{
    std::map<int, ICommand*>::iterator it = m_commands.find(key);
    if (it != m_commands.end()) {
        if (m_ownsCommands) {
            delete it->second;
        }
        m_commands.erase(it);
    }
}

void CommandInvoker::clearCommands()
{
    if (m_ownsCommands) {
        for (std::map<int, ICommand*>::iterator it = m_commands.begin();
             it != m_commands.end(); ++it) {
            delete it->second;
        }
    }
    m_commands.clear();
}
bool CommandInvoker::executeCommand(int key)
{
    std::map<int, ICommand*>::iterator it = m_commands.find(key);
    if (it != m_commands.end() && it->second) {
        ICommand* cmd = it->second;
        cmd->execute();
        if (cmd->canUndo()) {
            m_history.push_back(cmd);
        }

        return true;
    }
    return false;
}

bool CommandInvoker::hasCommand(int key) const
{
    return m_commands.find(key) != m_commands.end();
}
void CommandInvoker::undoLastCommand()
{
    if (!m_history.empty()) {
        ICommand* cmd = m_history.back();
        m_history.pop_back();
        cmd->undo();
    }
}

bool CommandInvoker::canUndo() const
{
    return !m_history.empty();
}

size_t CommandInvoker::getHistorySize() const
{
    return m_history.size();
}

void CommandInvoker::clearHistory()
{
    m_history.clear();
}
std::vector<int> CommandInvoker::getRegisteredKeys() const
{
    std::vector<int> keys;
    for (std::map<int, ICommand*>::const_iterator it = m_commands.begin();
         it != m_commands.end(); ++it) {
        keys.push_back(it->first);
    }
    return keys;
}

ICommand* CommandInvoker::getCommand(int key) const
{
    std::map<int, ICommand*>::const_iterator it = m_commands.find(key);
    if (it != m_commands.end()) {
        return it->second;
    }
    return 0;
}

std::string CommandInvoker::getCommandName(int key) const
{
    ICommand* cmd = getCommand(key);
    if (cmd) {
        return cmd->getName();
    }
    return "";
}

std::string CommandInvoker::getCommandDescription(int key) const
{
    ICommand* cmd = getCommand(key);
    if (cmd) {
        return cmd->getDescription();
    }
    return "";
}

std::vector<std::string> CommandInvoker::getCommandList() const
{
    std::vector<std::string> list;

    for (std::map<int, ICommand*>::const_iterator it = m_commands.begin();
         it != m_commands.end(); ++it) {
        std::ostringstream oss;
        oss << "[" << it->first << "] " << it->second->getName();
        list.push_back(oss.str());
    }

    return list;
}

}
