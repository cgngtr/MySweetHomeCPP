#include "SoundSystem.h"
#include <sstream>

namespace MySweetHome {

SoundSystem::SoundSystem(uint32_t id, const std::string& name, const std::string& location)
    : Device(id, name, DEVICE_SOUND_SYSTEM, location)
    , m_volume(50)
    , m_muted(false)
    , m_previousVolume(50)
    , m_playing(false)
    , m_source("Radio")
{
}

SoundSystem::~SoundSystem()
{
}

void SoundSystem::setVolume(uint8_t volume) {
    m_volume = (volume > 100) ? 100 : volume;
    if (m_volume > 0) {
        m_muted = false;
    }
}

uint8_t SoundSystem::getVolume() const {
    return m_volume;
}

void SoundSystem::mute() {
    if (!m_muted) {
        m_previousVolume = m_volume;
        m_volume = 0;
        m_muted = true;
    }
}

void SoundSystem::unmute() {
    if (m_muted) {
        m_volume = m_previousVolume;
        m_muted = false;
    }
}

bool SoundSystem::isMuted() const {
    return m_muted;
}

void SoundSystem::volumeUp() {
    if (m_muted) {
        unmute();
    }
    if (m_volume < 100) {
        m_volume++;
    }
}

void SoundSystem::volumeDown() {
    if (m_volume > 0) {
        m_volume--;
    }
    if (m_volume == 0) {
        m_muted = true;
    }
}

void SoundSystem::play() {
    if (isOn()) {
        m_playing = true;
    }
}

void SoundSystem::pause() {
    m_playing = false;
}

void SoundSystem::stop() {
    m_playing = false;
}

bool SoundSystem::isPlaying() const {
    return m_playing;
}

void SoundSystem::setSource(const std::string& source) {
    m_source = source;
}

std::string SoundSystem::getSource() const {
    return m_source;
}

Device* SoundSystem::clone() const {
    SoundSystem* newSystem = new SoundSystem(getId(), getName(), getLocation());
    newSystem->setVolume(m_volume);
    newSystem->setSource(m_source);
    return newSystem;
}

void SoundSystem::turnOn() {
    Device::turnOn();
}

void SoundSystem::turnOff() {
    Device::turnOff();
    m_playing = false;
}

std::string SoundSystem::getInfo() const {
    std::ostringstream oss;
    oss << Device::getInfo()
        << " | Ses: " << static_cast<int>(m_volume)
        << (m_muted ? " (Sessiz)" : "")
        << " | Kaynak: " << m_source
        << " | " << (m_playing ? "Caliniyor" : "Durduruldu");
    return oss.str();
}
SonySoundSystem::SonySoundSystem(uint32_t id, const std::string& location)
    : SoundSystem(id, "Sony Sound System", location)
    , m_surroundSound(true)
    , m_bassLevel(0)
{
}

SonySoundSystem::~SonySoundSystem()
{
}

Device* SonySoundSystem::clone() const {
    SonySoundSystem* newSystem = new SonySoundSystem(getId(), getLocation());
    newSystem->setVolume(m_volume);
    newSystem->setSource(m_source);
    newSystem->enableSurroundSound(m_surroundSound);
    newSystem->setBassLevel(m_bassLevel);
    return newSystem;
}

std::string SonySoundSystem::getInfo() const {
    std::ostringstream oss;
    oss << SoundSystem::getInfo()
        << " | Marka: Sony"
        << " | Surround: " << (m_surroundSound ? "Aktif" : "Pasif")
        << " | Bass: " << m_bassLevel;
    return oss.str();
}

void SonySoundSystem::enableSurroundSound(bool enable) {
    m_surroundSound = enable;
}

bool SonySoundSystem::hasSurroundSound() const {
    return m_surroundSound;
}

void SonySoundSystem::setBassLevel(int level) {
    if (level >= -10 && level <= 10) {
        m_bassLevel = level;
    }
}

int SonySoundSystem::getBassLevel() const {
    return m_bassLevel;
}
BoseSoundSystem::BoseSoundSystem(uint32_t id, const std::string& location)
    : SoundSystem(id, "Bose Sound System", location)
    , m_noiseCancel(true)
    , m_bluetoothEnabled(true)
{
}

BoseSoundSystem::~BoseSoundSystem()
{
}

Device* BoseSoundSystem::clone() const {
    BoseSoundSystem* newSystem = new BoseSoundSystem(getId(), getLocation());
    newSystem->setVolume(m_volume);
    newSystem->setSource(m_source);
    newSystem->enableNoiseCancel(m_noiseCancel);
    newSystem->enableBluetooth(m_bluetoothEnabled);
    return newSystem;
}

std::string BoseSoundSystem::getInfo() const {
    std::ostringstream oss;
    oss << SoundSystem::getInfo()
        << " | Marka: Bose"
        << " | Noise Cancel: " << (m_noiseCancel ? "Aktif" : "Pasif")
        << " | Bluetooth: " << (m_bluetoothEnabled ? "Aktif" : "Pasif");
    return oss.str();
}

void BoseSoundSystem::enableNoiseCancel(bool enable) {
    m_noiseCancel = enable;
}

bool BoseSoundSystem::hasNoiseCancel() const {
    return m_noiseCancel;
}

void BoseSoundSystem::enableBluetooth(bool enable) {
    m_bluetoothEnabled = enable;
}

bool BoseSoundSystem::isBluetoothEnabled() const {
    return m_bluetoothEnabled;
}
JBLSoundSystem::JBLSoundSystem(uint32_t id, const std::string& location)
    : SoundSystem(id, "JBL Sound System", location)
    , m_partyMode(false)
{
}

JBLSoundSystem::~JBLSoundSystem()
{
}

Device* JBLSoundSystem::clone() const {
    JBLSoundSystem* newSystem = new JBLSoundSystem(getId(), getLocation());
    newSystem->setVolume(m_volume);
    newSystem->setSource(m_source);
    newSystem->enablePartyMode(m_partyMode);
    return newSystem;
}

std::string JBLSoundSystem::getInfo() const {
    std::ostringstream oss;
    oss << SoundSystem::getInfo()
        << " | Marka: JBL"
        << " | Party Mode: " << (m_partyMode ? "Aktif" : "Pasif");
    return oss.str();
}

void JBLSoundSystem::enablePartyMode(bool enable) {
    m_partyMode = enable;
}

bool JBLSoundSystem::isPartyModeEnabled() const {
    return m_partyMode;
}

}
