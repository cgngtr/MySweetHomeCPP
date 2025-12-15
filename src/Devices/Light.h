#ifndef LIGHT_H
#define LIGHT_H

#include "Device.h"

namespace MySweetHome {

class Light : public Device {
public:
    Light(uint32_t id, const std::string& name, const std::string& location);
    virtual ~Light();
    void setBrightness(uint8_t level);
    uint8_t getBrightness() const;

    void setColor(uint8_t r, uint8_t g, uint8_t b);
    void getColor(uint8_t& r, uint8_t& g, uint8_t& b) const;
    virtual Device* clone() const;
    virtual void turnOn();
    virtual void turnOff();
    virtual std::string getInfo() const;

protected:
    uint8_t m_brightness;
    uint8_t m_colorR;
    uint8_t m_colorG;
    uint8_t m_colorB;
};
class ChinaLight : public Light {
public:
    ChinaLight(uint32_t id, const std::string& name, const std::string& location);
    virtual ~ChinaLight();

    virtual Device* clone() const;
    virtual std::string getInfo() const;

    std::string getConnectorType() const;

private:
    std::string m_connectorType;
};
class PhilipsLight : public Light {
public:
    PhilipsLight(uint32_t id, const std::string& name, const std::string& location);
    virtual ~PhilipsLight();

    virtual Device* clone() const;
    virtual std::string getInfo() const;
    void setColorTemperature(uint16_t kelvin);
    uint16_t getColorTemperature() const;
    void enableZigbee(bool enable);
    bool isZigbeeEnabled() const;

private:
    uint16_t m_colorTemperature;
    bool m_zigbeeEnabled;
};
class IKEALight : public Light {
public:
    IKEALight(uint32_t id, const std::string& name, const std::string& location);
    virtual ~IKEALight();

    virtual Device* clone() const;
    virtual std::string getInfo() const;
    void setWarmWhite(bool warm);
    bool isWarmWhite() const;

private:
    bool m_warmWhite;
};

}

#endif
