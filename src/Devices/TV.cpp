#include "TV.h"
#include <sstream>

namespace MySweetHome {
TV::TV(uint32_t id, const std::string& name, const std::string& location)
    : Device(id, name, DEVICE_TV, location)
    , m_channel(1)
    , m_volume(30)
    , m_muted(false)
    , m_previousVolume(30)
{
}

TV::~TV()
{
}

void TV::setChannel(uint16_t channel) {
    if (channel > 0 && channel <= 999) {
        m_channel = channel;
    }
}

uint16_t TV::getChannel() const {
    return m_channel;
}

void TV::setVolume(uint8_t volume) {
    m_volume = (volume > 100) ? 100 : volume;
    if (m_volume > 0) {
        m_muted = false;
    }
}

uint8_t TV::getVolume() const {
    return m_volume;
}

void TV::mute() {
    if (!m_muted) {
        m_previousVolume = m_volume;
        m_volume = 0;
        m_muted = true;
    }
}

void TV::unmute() {
    if (m_muted) {
        m_volume = m_previousVolume;
        m_muted = false;
    }
}

bool TV::isMuted() const {
    return m_muted;
}

void TV::volumeUp() {
    if (m_muted) {
        unmute();
    }
    if (m_volume < 100) {
        m_volume++;
    }
}

void TV::volumeDown() {
    if (m_volume > 0) {
        m_volume--;
    }
    if (m_volume == 0) {
        m_muted = true;
    }
}

void TV::channelUp() {
    if (m_channel < 999) {
        m_channel++;
    } else {
        m_channel = 1;
    }
}

void TV::channelDown() {
    if (m_channel > 1) {
        m_channel--;
    } else {
        m_channel = 999;
    }
}

Device* TV::clone() const {
    TV* newTV = new TV(getId(), getName(), getLocation());
    newTV->setChannel(m_channel);
    newTV->setVolume(m_volume);
    return newTV;
}

void TV::turnOn() {
    Device::turnOn();
}

void TV::turnOff() {
    Device::turnOff();
}

std::string TV::getInfo() const {
    std::ostringstream oss;
    oss << Device::getInfo()
        << " | Kanal: " << m_channel
        << " | Ses: " << static_cast<int>(m_volume)
        << (m_muted ? " (Sessiz)" : "");
    return oss.str();
}
SamsungTV::SamsungTV(uint32_t id, const std::string& location)
    : TV(id, "Samsung TV", location)
    , m_smartFeatures(true)
{
}

SamsungTV::~SamsungTV()
{
}

Device* SamsungTV::clone() const {
    SamsungTV* newTV = new SamsungTV(getId(), getLocation());
    newTV->setChannel(m_channel);
    newTV->setVolume(m_volume);
    newTV->enableSmartFeatures(m_smartFeatures);
    return newTV;
}

std::string SamsungTV::getInfo() const {
    std::ostringstream oss;
    oss << TV::getInfo()
        << " | Marka: Samsung"
        << " | Smart: " << (m_smartFeatures ? "Evet" : "Hayir");
    return oss.str();
}

void SamsungTV::enableSmartFeatures(bool enable) {
    m_smartFeatures = enable;
}

bool SamsungTV::hasSmartFeatures() const {
    return m_smartFeatures;
}
LGTV::LGTV(uint32_t id, const std::string& location)
    : TV(id, "LG TV", location)
    , m_webOS(true)
{
}

LGTV::~LGTV()
{
}

Device* LGTV::clone() const {
    LGTV* newTV = new LGTV(getId(), getLocation());
    newTV->setChannel(m_channel);
    newTV->setVolume(m_volume);
    newTV->enableWebOS(m_webOS);
    return newTV;
}

std::string LGTV::getInfo() const {
    std::ostringstream oss;
    oss << TV::getInfo()
        << " | Marka: LG"
        << " | WebOS: " << (m_webOS ? "Evet" : "Hayir");
    return oss.str();
}

void LGTV::enableWebOS(bool enable) {
    m_webOS = enable;
}

bool LGTV::hasWebOS() const {
    return m_webOS;
}

}
