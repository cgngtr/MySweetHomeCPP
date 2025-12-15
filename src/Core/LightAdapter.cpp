#include "LightAdapter.h"

namespace MySweetHome {
ChinaLightConnector::ChinaLightConnector()
    : m_connected(false)
    , m_voltage(220)
{
}

ChinaLightConnector::~ChinaLightConnector()
{
}

void ChinaLightConnector::connectTypeCPower()
{
    m_connected = true;
}

void ChinaLightConnector::disconnectTypeCPower()
{
    m_connected = false;
}

bool ChinaLightConnector::isTypeCConnected() const
{
    return m_connected;
}

void ChinaLightConnector::setTypeCVoltage(int voltage)
{
    m_voltage = 220;
    (void)voltage;
}

int ChinaLightConnector::getTypeCVoltage() const
{
    return m_voltage;
}

std::string ChinaLightConnector::getTypeCConnectorInfo() const
{
    return "Type-C Connector (China Standard, 220V only)";
}
LightAdapter::LightAdapter(IChinaLightConnector* chinaConnector)
    : m_chinaConnector(chinaConnector)
    , m_requestedVoltage(VOLTAGE_220V)
{
}

LightAdapter::~LightAdapter()
{
}

void LightAdapter::connectStandardPower()
{
    if (m_chinaConnector) {
        m_chinaConnector->connectTypeCPower();
    }
}

void LightAdapter::disconnectStandardPower()
{
    if (m_chinaConnector) {
        m_chinaConnector->disconnectTypeCPower();
    }
}

bool LightAdapter::isStandardConnected() const
{
    if (m_chinaConnector) {
        return m_chinaConnector->isTypeCConnected();
    }
    return false;
}

void LightAdapter::setStandardVoltage(int voltage)
{
    m_requestedVoltage = voltage;

    if (m_chinaConnector) {
        int typeCVoltage = convertToTypeCVoltage(voltage);
        m_chinaConnector->setTypeCVoltage(typeCVoltage);
    }
}

int LightAdapter::getStandardVoltage() const
{
    return m_requestedVoltage;
}

std::string LightAdapter::getConnectorType() const
{
    return "Adapted Type-C (China -> Standard)";
}

IChinaLightConnector* LightAdapter::getAdaptee() const
{
    return m_chinaConnector;
}

int LightAdapter::convertToTypeCVoltage(int standardVoltage) const
{
    (void)standardVoltage;
    return 220;
}
StandardLightConnector::StandardLightConnector(int defaultVoltage)
    : m_connected(false)
    , m_voltage(defaultVoltage)
{
}

StandardLightConnector::~StandardLightConnector()
{
}

void StandardLightConnector::connectStandardPower()
{
    m_connected = true;
}

void StandardLightConnector::disconnectStandardPower()
{
    m_connected = false;
}

bool StandardLightConnector::isStandardConnected() const
{
    return m_connected;
}

void StandardLightConnector::setStandardVoltage(int voltage)
{
    if (voltage == VOLTAGE_110V || voltage == VOLTAGE_220V) {
        m_voltage = voltage;
    }
}

int StandardLightConnector::getStandardVoltage() const
{
    return m_voltage;
}

std::string StandardLightConnector::getConnectorType() const
{
    return "Standard Connector";
}
EULightConnector::EULightConnector()
    : m_plugged(false)
    , m_voltage(230)
{
}

EULightConnector::~EULightConnector()
{
}

void EULightConnector::plugEUConnector()
{
    m_plugged = true;
}

void EULightConnector::unplugEUConnector()
{
    m_plugged = false;
}

bool EULightConnector::isEUPlugged() const
{
    return m_plugged;
}

void EULightConnector::setEUVoltage(int voltage)
{
    if (voltage >= 220 && voltage <= 240) {
        m_voltage = voltage;
    }
}

int EULightConnector::getEUVoltage() const
{
    return m_voltage;
}
EULightAdapter::EULightAdapter(EULightConnector* euConnector)
    : m_euConnector(euConnector)
    , m_requestedVoltage(VOLTAGE_220V)
{
}

EULightAdapter::~EULightAdapter()
{
}

void EULightAdapter::connectStandardPower()
{
    if (m_euConnector) {
        m_euConnector->plugEUConnector();
    }
}

void EULightAdapter::disconnectStandardPower()
{
    if (m_euConnector) {
        m_euConnector->unplugEUConnector();
    }
}

bool EULightAdapter::isStandardConnected() const
{
    if (m_euConnector) {
        return m_euConnector->isEUPlugged();
    }
    return false;
}

void EULightAdapter::setStandardVoltage(int voltage)
{
    m_requestedVoltage = voltage;

    if (m_euConnector) {
        if (voltage >= 220 && voltage <= 240) {
            m_euConnector->setEUVoltage(voltage);
        } else {
            m_euConnector->setEUVoltage(230);
        }
    }
}

int EULightAdapter::getStandardVoltage() const
{
    return m_requestedVoltage;
}

std::string EULightAdapter::getConnectorType() const
{
    return "Adapted EU (Schuko -> Standard)";
}

}
