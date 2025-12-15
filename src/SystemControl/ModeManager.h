#ifndef MODE_MANAGER_H
#define MODE_MANAGER_H

#include <string>
#include <vector>
#include "common_types.h"

namespace MySweetHome {

class Device;
class ModeManager {
public:
    ModeManager();
    ~ModeManager();
    void setMode(SystemMode mode);
    SystemMode getCurrentMode() const;
    std::string getCurrentModeString() const;
    bool shouldLightBeOn() const;
    bool shouldTVBeOn() const;
    bool shouldMusicBeOn() const;
    void applyModeToDevices(std::vector<Device*>& devices);

private:
    SystemMode m_currentMode;
};

}

#endif
