#ifndef LIGHT_ADAPTER_H
#define LIGHT_ADAPTER_H

#include "ILightAdapter.h"
#include <string>

namespace MySweetHome {
class IChinaLightConnector {
public:
    virtual ~IChinaLightConnector() {}
    virtual void connectTypeCPower() = 0;
    virtual void disconnectTypeCPower() = 0;
    virtual bool isTypeCConnected() const = 0;
    virtual void setTypeCVoltage(int voltage) = 0;
    virtual int getTypeCVoltage() const = 0;
};
class ChinaLightConnector : public IChinaLightConnector {
public:
    ChinaLightConnector();
    virtual ~ChinaLightConnector();
    virtual void connectTypeCPower();
    virtual void disconnectTypeCPower();
    virtual bool isTypeCConnected() const;
    virtual void setTypeCVoltage(int voltage);
    virtual int getTypeCVoltage() const;
    std::string getTypeCConnectorInfo() const;

private:
    bool m_connected;
    int m_voltage;
};
class LightAdapter : public IStandardLightConnector {
public:
    LightAdapter(IChinaLightConnector* chinaConnector);
    virtual ~LightAdapter();
    virtual void connectStandardPower();
    virtual void disconnectStandardPower();
    virtual bool isStandardConnected() const;
    virtual void setStandardVoltage(int voltage);
    virtual int getStandardVoltage() const;
    virtual std::string getConnectorType() const;
    IChinaLightConnector* getAdaptee() const;

private:
    IChinaLightConnector* m_chinaConnector;
    int m_requestedVoltage;
    int convertToTypeCVoltage(int standardVoltage) const;
};
class StandardLightConnector : public IStandardLightConnector {
public:
    StandardLightConnector(int defaultVoltage = VOLTAGE_220V);
    virtual ~StandardLightConnector();

    virtual void connectStandardPower();
    virtual void disconnectStandardPower();
    virtual bool isStandardConnected() const;
    virtual void setStandardVoltage(int voltage);
    virtual int getStandardVoltage() const;
    virtual std::string getConnectorType() const;

private:
    bool m_connected;
    int m_voltage;
};
class EULightConnector {
public:
    EULightConnector();
    ~EULightConnector();

    void plugEUConnector();
    void unplugEUConnector();
    bool isEUPlugged() const;
    void setEUVoltage(int voltage);
    int getEUVoltage() const;

private:
    bool m_plugged;
    int m_voltage;
};
class EULightAdapter : public IStandardLightConnector {
public:
    EULightAdapter(EULightConnector* euConnector);
    virtual ~EULightAdapter();

    virtual void connectStandardPower();
    virtual void disconnectStandardPower();
    virtual bool isStandardConnected() const;
    virtual void setStandardVoltage(int voltage);
    virtual int getStandardVoltage() const;
    virtual std::string getConnectorType() const;

private:
    EULightConnector* m_euConnector;
    int m_requestedVoltage;
};

}

#endif
