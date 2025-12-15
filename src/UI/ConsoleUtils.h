#ifndef CONSOLE_UTILS_H
#define CONSOLE_UTILS_H

#include <string>
#include <iostream>

namespace MySweetHome {
class ConsoleUtils {
public:
    static void clearScreen();
    static void setColor(int colorCode);
    static void resetColor();
    static void printColored(const std::string& text, int colorCode);
    static void printSuccess(const std::string& text);
    static void printError(const std::string& text);
    static void printWarning(const std::string& text);
    static void printInfo(const std::string& text);
    static void printHeader(const std::string& title);
    static void printSeparator(char ch, int length);
    static std::string getInput(const std::string& prompt);
    static int getIntInput(const std::string& prompt);
    static bool getYesNoInput(const std::string& prompt);
    static void pause(const std::string& message);
    static void pause();
    static const int COLOR_RESET = 0;
    static const int COLOR_RED = 31;
    static const int COLOR_GREEN = 32;
    static const int COLOR_YELLOW = 33;
    static const int COLOR_BLUE = 34;
    static const int COLOR_MAGENTA = 35;
    static const int COLOR_CYAN = 36;
    static const int COLOR_WHITE = 37;

private:
    ConsoleUtils();
};

}

#endif
