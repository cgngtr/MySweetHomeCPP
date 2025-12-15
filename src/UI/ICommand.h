#ifndef ICOMMAND_H
#define ICOMMAND_H

#include <string>

namespace MySweetHome {

class SmartHome;
class Menu;
class ICommand {
public:
    virtual ~ICommand() {}
    virtual void execute() = 0;
    virtual void undo() {}
    virtual bool canUndo() const { return false; }
    virtual std::string getName() const = 0;
    virtual std::string getDescription() const = 0;
    virtual int getMenuKey() const = 0;
};

}

#endif
