#ifndef TV_H
#define TV_H

#include "Device.h"

namespace MySweetHome {

class TV : public Device {
public:
    TV(uint32_t id, const std::string& name, const std::string& location);
    virtual ~TV();
    void setChannel(uint16_t channel);
    uint16_t getChannel() const;

    void setVolume(uint8_t volume);
    uint8_t getVolume() const;

    void mute();
    void unmute();
    bool isMuted() const;

    void volumeUp();
    void volumeDown();
    void channelUp();
    void channelDown();
    virtual Device* clone() const;
    virtual void turnOn();
    virtual void turnOff();
    virtual std::string getInfo() const;

protected:
    uint16_t m_channel;
    uint8_t m_volume;
    bool m_muted;
    uint8_t m_previousVolume;
};
class SamsungTV : public TV {
public:
    SamsungTV(uint32_t id, const std::string& location);
    virtual ~SamsungTV();

    virtual Device* clone() const;
    virtual std::string getInfo() const;

    void enableSmartFeatures(bool enable);
    bool hasSmartFeatures() const;

private:
    bool m_smartFeatures;
};
class LGTV : public TV {
public:
    LGTV(uint32_t id, const std::string& location);
    virtual ~LGTV();

    virtual Device* clone() const;
    virtual std::string getInfo() const;

    void enableWebOS(bool enable);
    bool hasWebOS() const;

private:
    bool m_webOS;
};

}

#endif
