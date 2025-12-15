#ifndef MENU_COMMANDS_H
#define MENU_COMMANDS_H

#include "ICommand.h"
#include "common_types.h"
#include <string>

namespace MySweetHome {

class SmartHome;
class Menu;
class CommandInvoker;
class MenuCommand : public ICommand {
public:
    MenuCommand(SmartHome* home, Menu* menu, int menuKey);
    virtual ~MenuCommand();

    virtual int getMenuKey() const;

protected:
    SmartHome* m_smartHome;
    Menu* m_menu;
    int m_menuKey;
};
class GetHomeStatusCommand : public MenuCommand {
public:
    GetHomeStatusCommand(SmartHome* home, Menu* menu);
    virtual ~GetHomeStatusCommand();

    virtual void execute();
    virtual std::string getName() const;
    virtual std::string getDescription() const;
};
class AddDeviceCommand : public MenuCommand {
public:
    AddDeviceCommand(SmartHome* home, Menu* menu);
    virtual ~AddDeviceCommand();

    virtual void execute();
    virtual void undo();
    virtual bool canUndo() const;
    virtual std::string getName() const;
    virtual std::string getDescription() const;

private:
    uint32_t m_lastAddedDeviceId;
    bool m_hasAddedDevice;
};
class RemoveDeviceCommand : public MenuCommand {
public:
    RemoveDeviceCommand(SmartHome* home, Menu* menu);
    virtual ~RemoveDeviceCommand();

    virtual void execute();
    virtual std::string getName() const;
    virtual std::string getDescription() const;
};
class PowerOnDeviceCommand : public MenuCommand {
public:
    PowerOnDeviceCommand(SmartHome* home, Menu* menu);
    virtual ~PowerOnDeviceCommand();

    virtual void execute();
    virtual void undo();
    virtual bool canUndo() const;
    virtual std::string getName() const;
    virtual std::string getDescription() const;

private:
    uint32_t m_lastDeviceId;
    bool m_hasPoweredOn;
};
class PowerOffDeviceCommand : public MenuCommand {
public:
    PowerOffDeviceCommand(SmartHome* home, Menu* menu);
    virtual ~PowerOffDeviceCommand();

    virtual void execute();
    virtual void undo();
    virtual bool canUndo() const;
    virtual std::string getName() const;
    virtual std::string getDescription() const;

private:
    uint32_t m_lastDeviceId;
    bool m_hasPoweredOff;
};
class ChangeModeCommand : public MenuCommand {
public:
    ChangeModeCommand(SmartHome* home, Menu* menu);
    virtual ~ChangeModeCommand();

    virtual void execute();
    virtual void undo();
    virtual bool canUndo() const;
    virtual std::string getName() const;
    virtual std::string getDescription() const;

private:
    SystemMode m_previousMode;
    bool m_hasChangedMode;
};
class ChangeStateCommand : public MenuCommand {
public:
    ChangeStateCommand(SmartHome* home, Menu* menu);
    virtual ~ChangeStateCommand();

    virtual void execute();
    virtual void undo();
    virtual bool canUndo() const;
    virtual std::string getName() const;
    virtual std::string getDescription() const;

private:
    SystemState m_previousState;
    bool m_hasChangedState;
};
class ShowManualCommand : public MenuCommand {
public:
    ShowManualCommand(SmartHome* home, Menu* menu);
    virtual ~ShowManualCommand();

    virtual void execute();
    virtual std::string getName() const;
    virtual std::string getDescription() const;
};
class ShowAboutCommand : public MenuCommand {
public:
    ShowAboutCommand(SmartHome* home, Menu* menu);
    virtual ~ShowAboutCommand();

    virtual void execute();
    virtual std::string getName() const;
    virtual std::string getDescription() const;
};
class ShutdownCommand : public MenuCommand {
public:
    ShutdownCommand(SmartHome* home, Menu* menu, bool* runningFlag);
    virtual ~ShutdownCommand();

    virtual void execute();
    virtual std::string getName() const;
    virtual std::string getDescription() const;

private:
    bool* m_runningFlag;
};
class SimulateSecurityEventCommand : public MenuCommand {
public:
    SimulateSecurityEventCommand(SmartHome* home, Menu* menu);
    virtual ~SimulateSecurityEventCommand();

    virtual void execute();
    virtual std::string getName() const;
    virtual std::string getDescription() const;
};
class SimulateDeviceFailureCommand : public MenuCommand {
public:
    SimulateDeviceFailureCommand(SmartHome* home, Menu* menu);
    virtual ~SimulateDeviceFailureCommand();

    virtual void execute();
    virtual std::string getName() const;
    virtual std::string getDescription() const;
};
class MenuCommandFactory {
public:
    static void registerAllCommands(CommandInvoker* invoker,
                                   SmartHome* home,
                                   Menu* menu,
                                   bool* runningFlag);
};

}

#endif
