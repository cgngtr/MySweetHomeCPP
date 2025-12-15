#ifndef MENU_H
#define MENU_H

#include <string>
#include "common_types.h"

namespace MySweetHome {

class SmartHome;
class MenuCommand;
class GetHomeStatusCommand;
class AddDeviceCommand;
class RemoveDeviceCommand;
class PowerOnDeviceCommand;
class PowerOffDeviceCommand;
class ChangeModeCommand;
class ChangeStateCommand;
class ShowManualCommand;
class ShowAboutCommand;
class ShutdownCommand;
class SimulateSecurityEventCommand;
class SimulateDeviceFailureCommand;
class Menu {
    friend class MenuCommand;
    friend class GetHomeStatusCommand;
    friend class AddDeviceCommand;
    friend class RemoveDeviceCommand;
    friend class PowerOnDeviceCommand;
    friend class PowerOffDeviceCommand;
    friend class ChangeModeCommand;
    friend class ChangeStateCommand;
    friend class ShowManualCommand;
    friend class ShowAboutCommand;
    friend class ShutdownCommand;
    friend class SimulateSecurityEventCommand;
    friend class SimulateDeviceFailureCommand;

public:
    Menu(SmartHome* smartHome);
    ~Menu();
    void run();

private:
    void showMainMenu();
    void printMenuOption(int number, const std::string& text);
    void getHomeStatus();
    void addDevice();
    void removeDevice();
    void powerOnDevice();
    void powerOffDevice();
    void changeMode();
    void changeState();
    void showManual();
    void showAbout();
    void shutdown();
    void simulateSecurityEvent();
    void simulateDeviceFailure();
    void listDevices();
    void listDevicesByType(DeviceType type);
    char getCharInput(const std::string& prompt);

    SmartHome* m_smartHome;
    bool m_running;
};

}

#endif
