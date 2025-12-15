#include "ConsoleUtils.h"
#include <cstdlib>
#include <cstdio>

#ifdef _WIN32
#include <windows.h>
#else
#include <unistd.h>
#endif

namespace MySweetHome {

void ConsoleUtils::clearScreen() {
#ifdef _WIN32
    std::system("cls");
#else
    std::cout << "\033[2J\033[1;1H";
#endif
}

void ConsoleUtils::setColor(int colorCode) {
#ifndef _WIN32
    std::cout << "\033[" << colorCode << "m";
#endif
}

void ConsoleUtils::resetColor() {
#ifndef _WIN32
    std::cout << "\033[0m";
#endif
}

void ConsoleUtils::printColored(const std::string& text, int colorCode) {
    setColor(colorCode);
    std::cout << text;
    resetColor();
}

void ConsoleUtils::printSuccess(const std::string& text) {
    printColored("[+] " + text, COLOR_GREEN);
    std::cout << std::endl;
}

void ConsoleUtils::printError(const std::string& text) {
    printColored("[X] " + text, COLOR_RED);
    std::cout << std::endl;
}

void ConsoleUtils::printWarning(const std::string& text) {
    printColored("[!] " + text, COLOR_YELLOW);
    std::cout << std::endl;
}

void ConsoleUtils::printInfo(const std::string& text) {
    printColored("[i] " + text, COLOR_CYAN);
    std::cout << std::endl;
}

void ConsoleUtils::printHeader(const std::string& title) {
    printSeparator('=', 60);
    setColor(COLOR_CYAN);
    std::cout << "  " << title << std::endl;
    resetColor();
    printSeparator('=', 60);
}

void ConsoleUtils::printSeparator(char ch, int length) {
    for (int i = 0; i < length; ++i) {
        std::cout << ch;
    }
    std::cout << std::endl;
}

std::string ConsoleUtils::getInput(const std::string& prompt) {
    std::cout << prompt;
    std::string input;
    std::getline(std::cin, input);
    return input;
}

int ConsoleUtils::getIntInput(const std::string& prompt) {
    std::cout << prompt;
    int value;
    while (!(std::cin >> value)) {
        std::cin.clear();
        while (std::cin.get() != '\n') { }
        printError("Gecersiz giris. Lutfen bir sayi girin.");
        std::cout << prompt;
    }
    while (std::cin.get() != '\n') { }
    return value;
}

bool ConsoleUtils::getYesNoInput(const std::string& prompt) {
    std::string input = getInput(prompt + " (E/H): ");
    return (input == "E" || input == "e" || input == "Evet" || input == "evet" ||
            input == "Y" || input == "y" || input == "Yes" || input == "yes");
}

void ConsoleUtils::pause(const std::string& message) {
    std::cout << message;
    std::cin.get();
}

void ConsoleUtils::pause() {
    pause("Devam etmek icin Enter'a basin...");
}

}
