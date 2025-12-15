#include "Light.h"
#include <sstream>

namespace MySweetHome {
Light::Light(uint32_t id, const std::string& name, const std::string& location)
    : Device(id, name, DEVICE_LIGHT, location)
    , m_brightness(100)
    , m_colorR(255)
    , m_colorG(255)
    , m_colorB(255)
{
}

Light::~Light()
{
}

void Light::setBrightness(uint8_t level) {
    m_brightness = (level > 100) ? 100 : level;
}

uint8_t Light::getBrightness() const {
    return m_brightness;
}

void Light::setColor(uint8_t r, uint8_t g, uint8_t b) {
    m_colorR = r;
    m_colorG = g;
    m_colorB = b;
}

void Light::getColor(uint8_t& r, uint8_t& g, uint8_t& b) const {
    r = m_colorR;
    g = m_colorG;
    b = m_colorB;
}

Device* Light::clone() const {
    Light* newLight = new Light(getId(), getName(), getLocation());
    newLight->setBrightness(m_brightness);
    newLight->setColor(m_colorR, m_colorG, m_colorB);
    return newLight;
}

void Light::turnOn() {
    Device::turnOn();
    if (m_brightness == 0) {
        m_brightness = 100;
    }
}

void Light::turnOff() {
    Device::turnOff();
}

std::string Light::getInfo() const {
    std::ostringstream oss;
    oss << Device::getInfo()
        << " | Parlaklik: %" << static_cast<int>(m_brightness)
        << " | Renk: RGB(" << static_cast<int>(m_colorR) << ","
        << static_cast<int>(m_colorG) << ","
        << static_cast<int>(m_colorB) << ")";
    return oss.str();
}
ChinaLight::ChinaLight(uint32_t id, const std::string& name, const std::string& location)
    : Light(id, name, location)
    , m_connectorType("Type-C Connector")
{
}

ChinaLight::~ChinaLight()
{
}

Device* ChinaLight::clone() const {
    ChinaLight* newLight = new ChinaLight(getId(), getName(), getLocation());
    newLight->setBrightness(m_brightness);
    newLight->setColor(m_colorR, m_colorG, m_colorB);
    return newLight;
}

std::string ChinaLight::getInfo() const {
    std::ostringstream oss;
    oss << Light::getInfo()
        << " | Connector: " << m_connectorType;
    return oss.str();
}

std::string ChinaLight::getConnectorType() const {
    return m_connectorType;
}
PhilipsLight::PhilipsLight(uint32_t id, const std::string& name, const std::string& location)
    : Light(id, name, location)
    , m_colorTemperature(4000)
    , m_zigbeeEnabled(true)
{
}

PhilipsLight::~PhilipsLight()
{
}

Device* PhilipsLight::clone() const {
    PhilipsLight* newLight = new PhilipsLight(getId(), getName(), getLocation());
    newLight->setBrightness(m_brightness);
    newLight->setColor(m_colorR, m_colorG, m_colorB);
    newLight->setColorTemperature(m_colorTemperature);
    newLight->enableZigbee(m_zigbeeEnabled);
    return newLight;
}

std::string PhilipsLight::getInfo() const {
    std::ostringstream oss;
    oss << Light::getInfo()
        << " | Marka: Philips Hue"
        << " | Renk Sicakligi: " << m_colorTemperature << "K"
        << " | Zigbee: " << (m_zigbeeEnabled ? "Aktif" : "Pasif");
    return oss.str();
}

void PhilipsLight::setColorTemperature(uint16_t kelvin) {
    if (kelvin >= 2000 && kelvin <= 6500) {
        m_colorTemperature = kelvin;
    }
}

uint16_t PhilipsLight::getColorTemperature() const {
    return m_colorTemperature;
}

void PhilipsLight::enableZigbee(bool enable) {
    m_zigbeeEnabled = enable;
}

bool PhilipsLight::isZigbeeEnabled() const {
    return m_zigbeeEnabled;
}
IKEALight::IKEALight(uint32_t id, const std::string& name, const std::string& location)
    : Light(id, name, location)
    , m_warmWhite(true)
{
}

IKEALight::~IKEALight()
{
}

Device* IKEALight::clone() const {
    IKEALight* newLight = new IKEALight(getId(), getName(), getLocation());
    newLight->setBrightness(m_brightness);
    newLight->setColor(m_colorR, m_colorG, m_colorB);
    newLight->setWarmWhite(m_warmWhite);
    return newLight;
}

std::string IKEALight::getInfo() const {
    std::ostringstream oss;
    oss << Light::getInfo()
        << " | Marka: IKEA Tradfri"
        << " | " << (m_warmWhite ? "Sicak Beyaz" : "Soguk Beyaz");
    return oss.str();
}

void IKEALight::setWarmWhite(bool warm) {
    m_warmWhite = warm;
}

bool IKEALight::isWarmWhite() const {
    return m_warmWhite;
}

}
