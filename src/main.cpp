#include <iostream>
#include "SmartHome.h"
#include "Menu.h"
#include "Logger.h"

int main() {
    MySweetHome::Logger::getInstance().setLogToFile(true);
    MySweetHome::Logger::getInstance().setLogFile("mysweethome.log");
    MySweetHome::Logger::getInstance().openLogFile();

    MySweetHome::Logger::getInstance().info("MySweetHome sistemi baslatiliyor...");
    MySweetHome::SmartHome smartHome;
    MySweetHome::Menu menu(&smartHome);
    menu.run();
    MySweetHome::Logger::getInstance().info("MySweetHome sistemi kapatiliyor...");
    MySweetHome::Logger::getInstance().closeLogFile();

    return 0;
}
