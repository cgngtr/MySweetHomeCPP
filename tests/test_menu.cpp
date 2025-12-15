#include <iostream>
#include <cassert>
#include "common_types.h"
#include "SmartHome.h"

using namespace MySweetHome;

void testSmartHome() {
    std::cout << "Testing SmartHome..." << std::endl;

    SmartHome smartHome;

    assert(smartHome.getDeviceCount() == 0);
    assert(smartHome.addLight("Light 1", "Living Room") != 0);
    assert(smartHome.addCamera("Camera 1", "Front Door") != 0);
    smartHome.addDetectorPair("Detector 1", "Kitchen");
    assert(smartHome.addSamsungTV("Bedroom") != 0);
    assert(smartHome.addAlarm("Alarm 1", "Main Entry") != 0);

    assert(smartHome.getDeviceCount() == 6);
    Device* device = smartHome.getDevice(1);
    assert(device != 0);
    assert(device->getName() == "Light 1");
    assert(smartHome.removeDevice(1));
    assert(smartHome.getDeviceCount() == 5);
    assert(smartHome.getDevice(1) == 0);
    assert(smartHome.getCurrentMode() == MODE_NORMAL);

    smartHome.setMode(MODE_EVENING);
    assert(smartHome.getCurrentMode() == MODE_EVENING);

    smartHome.setMode(MODE_PARTY);
    assert(smartHome.getCurrentMode() == MODE_PARTY);

    smartHome.setMode(MODE_CINEMA);
    assert(smartHome.getCurrentMode() == MODE_CINEMA);

    std::cout << "SmartHome tests passed!" << std::endl;
}

void testDeviceControl() {
    std::cout << "Testing Device Control..." << std::endl;

    SmartHome smartHome;

    smartHome.addLight("Light 1", "Room A");
    smartHome.addLight("Light 2", "Room A");
    smartHome.addLight("Light 3", "Room B");
    smartHome.turnAllOn();
    assert(smartHome.getActiveDeviceCount() == 3);
    smartHome.turnAllOff();
    assert(smartHome.getActiveDeviceCount() == 0);
    smartHome.turnOnByLocation("Room A");
    assert(smartHome.getActiveDeviceCount() == 2);
    smartHome.turnOffByLocation("Room A");
    assert(smartHome.getActiveDeviceCount() == 0);

    std::cout << "Device Control tests passed!" << std::endl;
}

void testStateManagement() {
    std::cout << "Testing State Management..." << std::endl;

    SmartHome smartHome;
    assert(smartHome.getCurrentState() == STATE_NORMAL);
    smartHome.setState(STATE_HIGH_PERFORMANCE);
    assert(smartHome.getCurrentState() == STATE_HIGH_PERFORMANCE);

    smartHome.setState(STATE_LOW_POWER);
    assert(smartHome.getCurrentState() == STATE_LOW_POWER);

    smartHome.setState(STATE_SLEEP);
    assert(smartHome.getCurrentState() == STATE_SLEEP);
    assert(smartHome.goToPreviousState());
    assert(smartHome.getCurrentState() == STATE_LOW_POWER);

    assert(smartHome.goToPreviousState());
    assert(smartHome.getCurrentState() == STATE_HIGH_PERFORMANCE);

    std::cout << "State Management tests passed!" << std::endl;
}

int main() {
    std::cout << "=== MySweetHome Menu/System Tests ===" << std::endl << std::endl;

    testSmartHome();
    testDeviceControl();
    testStateManagement();

    std::cout << std::endl << "All tests passed!" << std::endl;
    return 0;
}
