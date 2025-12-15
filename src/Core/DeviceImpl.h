#ifndef DEVICE_IMPL_H
#define DEVICE_IMPL_H

#include "IDeviceImpl.h"
#include <string>

namespace MySweetHome {
enum HardwareStatus {
    HW_STATUS_OK = 0,
    HW_STATUS_ERROR = 1,
    HW_STATUS_DISCONNECTED = 2,
    HW_STATUS_INITIALIZING = 3,
    HW_STATUS_BUSY = 4
};
class HardwareDeviceImpl : public IDeviceImpl {
public:
    HardwareDeviceImpl();
    virtual ~HardwareDeviceImpl();
    virtual void powerOn();
    virtual void powerOff();
    virtual bool isPowered() const;
    virtual int getHardwareStatus() const;
    virtual std::string getImplType() const;
    virtual std::string getImplInfo() const;
    virtual bool isConnected() const;
    virtual bool connect();
    virtual void disconnect();
    void setHardwareAddress(const std::string& address);
    std::string getHardwareAddress() const;
    void reset();

private:
    bool m_powered;
    bool m_connected;
    int m_hardwareStatus;
    std::string m_hardwareAddress;

    bool initializeHardware();
    void shutdownHardware();
};
class SimulatedDeviceImpl : public IDeviceImpl {
public:
    SimulatedDeviceImpl();
    virtual ~SimulatedDeviceImpl();
    virtual void powerOn();
    virtual void powerOff();
    virtual bool isPowered() const;
    virtual int getHardwareStatus() const;
    virtual std::string getImplType() const;
    virtual std::string getImplInfo() const;
    virtual bool isConnected() const;
    virtual bool connect();
    virtual void disconnect();
    void setSimulatedDelay(int milliseconds);
    int getSimulatedDelay() const;
    void setFailureRate(float rate);
    float getFailureRate() const;
    void simulateFailure();

private:
    bool m_powered;
    bool m_connected;
    int m_simulatedDelay;
    float m_failureRate;
    bool m_simulatedFailure;

    void simulateDelay() const;
    bool shouldFail() const;
};
class NetworkDeviceImpl : public IDeviceImpl {
public:
    NetworkDeviceImpl(const std::string& endpoint = "");
    virtual ~NetworkDeviceImpl();
    virtual void powerOn();
    virtual void powerOff();
    virtual bool isPowered() const;
    virtual int getHardwareStatus() const;
    virtual std::string getImplType() const;
    virtual std::string getImplInfo() const;
    virtual bool isConnected() const;
    virtual bool connect();
    virtual void disconnect();
    void setEndpoint(const std::string& endpoint);
    std::string getEndpoint() const;
    void setTimeout(int milliseconds);
    int getTimeout() const;
    bool ping() const;

private:
    std::string m_endpoint;
    int m_timeout;
    bool m_powered;
    bool m_connected;

    bool sendCommand(const std::string& command);
    std::string receiveResponse();
};
class MockDeviceImpl : public IDeviceImpl {
public:
    MockDeviceImpl();
    virtual ~MockDeviceImpl();
    virtual void powerOn();
    virtual void powerOff();
    virtual bool isPowered() const;
    virtual int getHardwareStatus() const;
    virtual std::string getImplType() const;
    virtual std::string getImplInfo() const;
    virtual bool isConnected() const;
    virtual bool connect();
    virtual void disconnect();
    int getPowerOnCallCount() const;
    int getPowerOffCallCount() const;
    int getConnectCallCount() const;
    void resetCallCounts();
    void setReturnPowered(bool powered);
    void setReturnConnected(bool connected);
    void setReturnStatus(int status);

private:
    bool m_powered;
    bool m_connected;
    int m_status;
    int m_powerOnCalls;
    int m_powerOffCalls;
    int m_connectCalls;
    bool m_returnPowered;
    bool m_returnConnected;
    int m_returnStatus;
};
class DeviceImplFactory {
public:
    enum ImplType {
        IMPL_HARDWARE,
        IMPL_SIMULATED,
        IMPL_NETWORK,
        IMPL_MOCK
    };

    static IDeviceImpl* createImpl(ImplType type);
    static IDeviceImpl* createNetworkImpl(const std::string& endpoint);
    static IDeviceImpl* createSimulatedImpl(int delayMs = 0);
};

}

#endif
