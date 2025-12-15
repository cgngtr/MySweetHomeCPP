#include <iostream>
#include <cassert>
#include "common_types.h"
#include "Light.h"
#include "Camera.h"
#include "Detector.h"
#include "TV.h"
#include "Alarm.h"

using namespace MySweetHome;

void testLight() {
    std::cout << "Testing Light..." << std::endl;

    Light light(1, "Test Light", "Living Room");

    assert(light.getId() == 1);
    assert(light.getName() == "Test Light");
    assert(light.getLocation() == "Living Room");
    assert(!light.isOn());

    light.turnOn();
    assert(light.isOn());
    assert(light.getBrightness() == 100);

    light.setBrightness(50);
    assert(light.getBrightness() == 50);

    light.setColor(255, 0, 0);
    uint8_t r, g, b;
    light.getColor(r, g, b);
    assert(r == 255 && g == 0 && b == 0);

    light.turnOff();
    assert(!light.isOn());

    std::cout << "Light tests passed!" << std::endl;
}

void testCamera() {
    std::cout << "Testing Camera..." << std::endl;

    Camera camera(2, "Test Camera", "Front Door");

    assert(!camera.isOn());
    assert(!camera.isRecording());

    camera.turnOn();
    assert(camera.isOn());

    camera.startRecording();
    assert(camera.isRecording());

    camera.stopRecording();
    assert(!camera.isRecording());

    camera.enableMotionDetection(true);
    assert(camera.isMotionDetectionEnabled());
    camera.turnOff();
    assert(camera.isOn());
    assert(camera.isCritical());

    std::cout << "Camera tests passed!" << std::endl;
}

void testDetector() {
    std::cout << "Testing Detector..." << std::endl;

    SmokeDetector smokeDetector(3, "Smoke Detector", "Kitchen");

    assert(!smokeDetector.isOn());
    assert(!smokeDetector.isAlarmTriggered());

    smokeDetector.turnOn();
    assert(smokeDetector.isOn());

    smokeDetector.testAlarm();
    assert(smokeDetector.isAlarmTriggered());

    smokeDetector.resetAlarm();
    assert(!smokeDetector.isAlarmTriggered());

    smokeDetector.setSensorValue(50.0f);
    assert(smokeDetector.isAlarmTriggered());

    std::cout << "Detector tests passed!" << std::endl;
}

void testTV() {
    std::cout << "Testing TV..." << std::endl;

    TV tv(4, "Living Room TV", "Living Room");

    assert(!tv.isOn());

    tv.turnOn();
    assert(tv.isOn());

    tv.setChannel(5);
    assert(tv.getChannel() == 5);

    tv.setVolume(50);
    assert(tv.getVolume() == 50);

    tv.mute();
    assert(tv.isMuted());
    assert(tv.getVolume() == 0);

    tv.unmute();
    assert(!tv.isMuted());
    assert(tv.getVolume() == 50);

    tv.volumeUp();
    assert(tv.getVolume() == 51);

    std::cout << "TV tests passed!" << std::endl;
}

void testAlarm() {
    std::cout << "Testing Alarm..." << std::endl;

    Alarm alarm(5, "Home Alarm", "Main Entry");

    assert(!alarm.isOn());
    assert(!alarm.isArmed());

    alarm.turnOn();
    assert(alarm.isOn());

    alarm.armAway();
    assert(alarm.isArmed());

    alarm.trigger(ALARM_INTRUSION);
    assert(alarm.isTriggered());

    alarm.disarm("wrong");
    assert(alarm.isTriggered());

    alarm.disarm("1234");
    assert(!alarm.isTriggered());
    assert(!alarm.isArmed());

    std::cout << "Alarm tests passed!" << std::endl;
}

int main() {
    std::cout << "=== MySweetHome Device Tests ===" << std::endl << std::endl;

    testLight();
    testCamera();
    testDetector();
    testTV();
    testAlarm();

    std::cout << std::endl << "All tests passed!" << std::endl;
    return 0;
}
