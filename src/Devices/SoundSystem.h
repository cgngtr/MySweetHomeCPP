#ifndef SOUND_SYSTEM_H
#define SOUND_SYSTEM_H

#include "Device.h"

namespace MySweetHome {

class SoundSystem : public Device {
public:
    SoundSystem(uint32_t id, const std::string& name, const std::string& location);
    virtual ~SoundSystem();
    void setVolume(uint8_t volume);
    uint8_t getVolume() const;

    void mute();
    void unmute();
    bool isMuted() const;

    void volumeUp();
    void volumeDown();
    void play();
    void pause();
    void stop();
    bool isPlaying() const;
    void setSource(const std::string& source);
    std::string getSource() const;
    virtual Device* clone() const;
    virtual void turnOn();
    virtual void turnOff();
    virtual std::string getInfo() const;

protected:
    uint8_t m_volume;
    bool m_muted;
    uint8_t m_previousVolume;
    bool m_playing;
    std::string m_source;
};
class SonySoundSystem : public SoundSystem {
public:
    SonySoundSystem(uint32_t id, const std::string& location);
    virtual ~SonySoundSystem();

    virtual Device* clone() const;
    virtual std::string getInfo() const;
    void enableSurroundSound(bool enable);
    bool hasSurroundSound() const;
    void setBassLevel(int level);
    int getBassLevel() const;

private:
    bool m_surroundSound;
    int m_bassLevel;
};
class BoseSoundSystem : public SoundSystem {
public:
    BoseSoundSystem(uint32_t id, const std::string& location);
    virtual ~BoseSoundSystem();

    virtual Device* clone() const;
    virtual std::string getInfo() const;
    void enableNoiseCancel(bool enable);
    bool hasNoiseCancel() const;
    void enableBluetooth(bool enable);
    bool isBluetoothEnabled() const;

private:
    bool m_noiseCancel;
    bool m_bluetoothEnabled;
};
class JBLSoundSystem : public SoundSystem {
public:
    JBLSoundSystem(uint32_t id, const std::string& location);
    virtual ~JBLSoundSystem();

    virtual Device* clone() const;
    virtual std::string getInfo() const;
    void enablePartyMode(bool enable);
    bool isPartyModeEnabled() const;

private:
    bool m_partyMode;
};

}

#endif
