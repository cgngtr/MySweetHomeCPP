#include "MenuCommands.h"
#include "CommandInvoker.h"
#include "Menu.h"
#include "SmartHome.h"
#include "ConsoleUtils.h"
#include "Logger.h"

namespace MySweetHome {
MenuCommand::MenuCommand(SmartHome* home, Menu* menu, int menuKey)
    : m_smartHome(home)
    , m_menu(menu)
    , m_menuKey(menuKey)
{
}

MenuCommand::~MenuCommand()
{
}

int MenuCommand::getMenuKey() const
{
    return m_menuKey;
}
GetHomeStatusCommand::GetHomeStatusCommand(SmartHome* home, Menu* menu)
    : MenuCommand(home, menu, 1)
{
}

GetHomeStatusCommand::~GetHomeStatusCommand()
{
}

void GetHomeStatusCommand::execute()
{
    if (m_menu) {
        m_menu->getHomeStatus();
    }
}

std::string GetHomeStatusCommand::getName() const
{
    return "Get Home Status";
}

std::string GetHomeStatusCommand::getDescription() const
{
    return "Display status of all devices in the home";
}
AddDeviceCommand::AddDeviceCommand(SmartHome* home, Menu* menu)
    : MenuCommand(home, menu, 2)
    , m_lastAddedDeviceId(0)
    , m_hasAddedDevice(false)
{
}

AddDeviceCommand::~AddDeviceCommand()
{
}

void AddDeviceCommand::execute()
{
    if (m_menu) {
        size_t countBefore = m_smartHome ? m_smartHome->getDeviceCount() : 0;

        m_menu->addDevice();
        if (m_smartHome && m_smartHome->getDeviceCount() > countBefore) {
            m_hasAddedDevice = true;
        }
    }
}

void AddDeviceCommand::undo()
{
    if (m_hasAddedDevice && m_smartHome && m_lastAddedDeviceId > 0) {
        m_smartHome->removeDevice(m_lastAddedDeviceId);
        m_hasAddedDevice = false;
        Logger::getInstance().info("Undo: Removed last added device");
    }
}

bool AddDeviceCommand::canUndo() const
{
    return m_hasAddedDevice;
}

std::string AddDeviceCommand::getName() const
{
    return "Add Device";
}

std::string AddDeviceCommand::getDescription() const
{
    return "Add a new device (Light/Detector/Camera/TV)";
}
RemoveDeviceCommand::RemoveDeviceCommand(SmartHome* home, Menu* menu)
    : MenuCommand(home, menu, 3)
{
}

RemoveDeviceCommand::~RemoveDeviceCommand()
{
}

void RemoveDeviceCommand::execute()
{
    if (m_menu) {
        m_menu->removeDevice();
    }
}

std::string RemoveDeviceCommand::getName() const
{
    return "Remove Device";
}

std::string RemoveDeviceCommand::getDescription() const
{
    return "Remove an existing device";
}
PowerOnDeviceCommand::PowerOnDeviceCommand(SmartHome* home, Menu* menu)
    : MenuCommand(home, menu, 4)
    , m_lastDeviceId(0)
    , m_hasPoweredOn(false)
{
}

PowerOnDeviceCommand::~PowerOnDeviceCommand()
{
}

void PowerOnDeviceCommand::execute()
{
    if (m_menu) {
        m_menu->powerOnDevice();
    }
}

void PowerOnDeviceCommand::undo()
{
    if (m_hasPoweredOn && m_smartHome && m_lastDeviceId > 0) {
        m_smartHome->powerOffDevice(m_lastDeviceId);
        m_hasPoweredOn = false;
        Logger::getInstance().info("Undo: Powered off device");
    }
}

bool PowerOnDeviceCommand::canUndo() const
{
    return m_hasPoweredOn;
}

std::string PowerOnDeviceCommand::getName() const
{
    return "Power On Device";
}

std::string PowerOnDeviceCommand::getDescription() const
{
    return "Turn on a device";
}
PowerOffDeviceCommand::PowerOffDeviceCommand(SmartHome* home, Menu* menu)
    : MenuCommand(home, menu, 5)
    , m_lastDeviceId(0)
    , m_hasPoweredOff(false)
{
}

PowerOffDeviceCommand::~PowerOffDeviceCommand()
{
}

void PowerOffDeviceCommand::execute()
{
    if (m_menu) {
        m_menu->powerOffDevice();
    }
}

void PowerOffDeviceCommand::undo()
{
    if (m_hasPoweredOff && m_smartHome && m_lastDeviceId > 0) {
        m_smartHome->powerOnDevice(m_lastDeviceId);
        m_hasPoweredOff = false;
        Logger::getInstance().info("Undo: Powered on device");
    }
}

bool PowerOffDeviceCommand::canUndo() const
{
    return m_hasPoweredOff;
}

std::string PowerOffDeviceCommand::getName() const
{
    return "Power Off Device";
}

std::string PowerOffDeviceCommand::getDescription() const
{
    return "Turn off a device (critical devices cannot be turned off)";
}
ChangeModeCommand::ChangeModeCommand(SmartHome* home, Menu* menu)
    : MenuCommand(home, menu, 6)
    , m_previousMode(MODE_NORMAL)
    , m_hasChangedMode(false)
{
}

ChangeModeCommand::~ChangeModeCommand()
{
}

void ChangeModeCommand::execute()
{
    if (m_smartHome) {
        m_previousMode = m_smartHome->getCurrentMode();
    }

    if (m_menu) {
        m_menu->changeMode();
        m_hasChangedMode = true;
    }
}

void ChangeModeCommand::undo()
{
    if (m_hasChangedMode && m_smartHome) {
        m_smartHome->setMode(m_previousMode);
        m_hasChangedMode = false;
        Logger::getInstance().info("Undo: Restored previous mode");
    }
}

bool ChangeModeCommand::canUndo() const
{
    return m_hasChangedMode;
}

std::string ChangeModeCommand::getName() const
{
    return "Change Mode";
}

std::string ChangeModeCommand::getDescription() const
{
    return "Change system mode (Normal/Evening/Party/Cinema)";
}
ChangeStateCommand::ChangeStateCommand(SmartHome* home, Menu* menu)
    : MenuCommand(home, menu, 7)
    , m_previousState(STATE_NORMAL)
    , m_hasChangedState(false)
{
}

ChangeStateCommand::~ChangeStateCommand()
{
}

void ChangeStateCommand::execute()
{
    if (m_smartHome) {
        m_previousState = m_smartHome->getCurrentState();
    }

    if (m_menu) {
        m_menu->changeState();
        m_hasChangedState = true;
    }
}

void ChangeStateCommand::undo()
{
    if (m_hasChangedState && m_smartHome) {
        m_smartHome->setState(m_previousState);
        m_hasChangedState = false;
        Logger::getInstance().info("Undo: Restored previous state");
    }
}

bool ChangeStateCommand::canUndo() const
{
    return m_hasChangedState;
}

std::string ChangeStateCommand::getName() const
{
    return "Change State";
}

std::string ChangeStateCommand::getDescription() const
{
    return "Change system state (Normal/High Performance/Low Power/Sleep)";
}
ShowManualCommand::ShowManualCommand(SmartHome* home, Menu* menu)
    : MenuCommand(home, menu, 8)
{
}

ShowManualCommand::~ShowManualCommand()
{
}

void ShowManualCommand::execute()
{
    if (m_menu) {
        m_menu->showManual();
    }
}

std::string ShowManualCommand::getName() const
{
    return "Manual";
}

std::string ShowManualCommand::getDescription() const
{
    return "Display the user manual";
}
ShowAboutCommand::ShowAboutCommand(SmartHome* home, Menu* menu)
    : MenuCommand(home, menu, 9)
{
}

ShowAboutCommand::~ShowAboutCommand()
{
}

void ShowAboutCommand::execute()
{
    if (m_menu) {
        m_menu->showAbout();
    }
}

std::string ShowAboutCommand::getName() const
{
    return "About";
}

std::string ShowAboutCommand::getDescription() const
{
    return "Display product information and developers";
}
ShutdownCommand::ShutdownCommand(SmartHome* home, Menu* menu, bool* runningFlag)
    : MenuCommand(home, menu, 10)
    , m_runningFlag(runningFlag)
{
}

ShutdownCommand::~ShutdownCommand()
{
}

void ShutdownCommand::execute()
{
    if (m_menu) {
        m_menu->shutdown();
    }
}

std::string ShutdownCommand::getName() const
{
    return "Shutdown";
}

std::string ShutdownCommand::getDescription() const
{
    return "Exit the application";
}
SimulateSecurityEventCommand::SimulateSecurityEventCommand(SmartHome* home, Menu* menu)
    : MenuCommand(home, menu, 11)
{
}

SimulateSecurityEventCommand::~SimulateSecurityEventCommand()
{
}

void SimulateSecurityEventCommand::execute()
{
    if (m_menu) {
        m_menu->simulateSecurityEvent();
    }
}

std::string SimulateSecurityEventCommand::getName() const
{
    return "Simulate Security Event";
}

std::string SimulateSecurityEventCommand::getDescription() const
{
    return "Simulate motion/smoke/gas detection (hidden option)";
}
SimulateDeviceFailureCommand::SimulateDeviceFailureCommand(SmartHome* home, Menu* menu)
    : MenuCommand(home, menu, 12)
{
}

SimulateDeviceFailureCommand::~SimulateDeviceFailureCommand()
{
}

void SimulateDeviceFailureCommand::execute()
{
    if (m_menu) {
        m_menu->simulateDeviceFailure();
    }
}

std::string SimulateDeviceFailureCommand::getName() const
{
    return "Simulate Device Failure";
}

std::string SimulateDeviceFailureCommand::getDescription() const
{
    return "Simulate device failure for notification testing (hidden option)";
}
void MenuCommandFactory::registerAllCommands(CommandInvoker* invoker,
                                             SmartHome* home,
                                             Menu* menu,
                                             bool* runningFlag)
{
    if (!invoker) return;
    invoker->registerCommand(1, new GetHomeStatusCommand(home, menu));
    invoker->registerCommand(2, new AddDeviceCommand(home, menu));
    invoker->registerCommand(3, new RemoveDeviceCommand(home, menu));
    invoker->registerCommand(4, new PowerOnDeviceCommand(home, menu));
    invoker->registerCommand(5, new PowerOffDeviceCommand(home, menu));
    invoker->registerCommand(6, new ChangeModeCommand(home, menu));
    invoker->registerCommand(7, new ChangeStateCommand(home, menu));
    invoker->registerCommand(8, new ShowManualCommand(home, menu));
    invoker->registerCommand(9, new ShowAboutCommand(home, menu));
    invoker->registerCommand(10, new ShutdownCommand(home, menu, runningFlag));
    invoker->registerCommand(11, new SimulateSecurityEventCommand(home, menu));
    invoker->registerCommand(12, new SimulateDeviceFailureCommand(home, menu));
}

}
