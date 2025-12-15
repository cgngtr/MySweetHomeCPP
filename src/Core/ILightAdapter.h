#ifndef ILIGHT_ADAPTER_H
#define ILIGHT_ADAPTER_H

#include <string>

namespace MySweetHome {
class IStandardLightConnector {
public:
    virtual ~IStandardLightConnector() {}
    virtual void connectStandardPower() = 0;
    virtual void disconnectStandardPower() = 0;
    virtual bool isStandardConnected() const = 0;
    virtual void setStandardVoltage(int voltage) = 0;
    virtual int getStandardVoltage() const = 0;
    virtual std::string getConnectorType() const = 0;
};
const int VOLTAGE_110V = 110;
const int VOLTAGE_220V = 220;
const int VOLTAGE_DEFAULT = VOLTAGE_220V;

}

#endif
