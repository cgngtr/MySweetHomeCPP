#include "DeviceImpl.h"
#include "Logger.h"
#include <sstream>
#include <cstdlib>

namespace MySweetHome {
HardwareDeviceImpl::HardwareDeviceImpl()
    : m_powered(false)
    , m_connected(false)
    , m_hardwareStatus(HW_STATUS_DISCONNECTED)
{
}

HardwareDeviceImpl::~HardwareDeviceImpl()
{
    if (m_connected) {
        shutdownHardware();
    }
}

void HardwareDeviceImpl::powerOn()
{
    if (!m_connected) {
        Logger::getInstance().warning("Hardware not connected, cannot power on");
        return;
    }
    m_powered = true;
    m_hardwareStatus = HW_STATUS_OK;
    Logger::getInstance().debug("Hardware powered on: " + m_hardwareAddress);
}

void HardwareDeviceImpl::powerOff()
{
    if (!m_connected) {
        return;
    }

    m_powered = false;
    Logger::getInstance().debug("Hardware powered off: " + m_hardwareAddress);
}

bool HardwareDeviceImpl::isPowered() const
{
    return m_powered;
}

int HardwareDeviceImpl::getHardwareStatus() const
{
    return m_hardwareStatus;
}

std::string HardwareDeviceImpl::getImplType() const
{
    return "Hardware";
}

std::string HardwareDeviceImpl::getImplInfo() const
{
    std::ostringstream oss;
    oss << "Hardware Implementation [Address: " << m_hardwareAddress
        << ", Status: " << m_hardwareStatus << "]";
    return oss.str();
}

bool HardwareDeviceImpl::isConnected() const
{
    return m_connected;
}

bool HardwareDeviceImpl::connect()
{
    if (m_connected) {
        return true;
    }

    if (initializeHardware()) {
        m_connected = true;
        m_hardwareStatus = HW_STATUS_OK;
        Logger::getInstance().info("Hardware connected: " + m_hardwareAddress);
        return true;
    }

    m_hardwareStatus = HW_STATUS_ERROR;
    return false;
}

void HardwareDeviceImpl::disconnect()
{
    if (m_connected) {
        shutdownHardware();
        m_connected = false;
        m_powered = false;
        m_hardwareStatus = HW_STATUS_DISCONNECTED;
        Logger::getInstance().info("Hardware disconnected: " + m_hardwareAddress);
    }
}

void HardwareDeviceImpl::setHardwareAddress(const std::string& address)
{
    m_hardwareAddress = address;
}

std::string HardwareDeviceImpl::getHardwareAddress() const
{
    return m_hardwareAddress;
}

void HardwareDeviceImpl::reset()
{
    disconnect();
    connect();
}

bool HardwareDeviceImpl::initializeHardware()
{
    m_hardwareStatus = HW_STATUS_INITIALIZING;
    return true;
}

void HardwareDeviceImpl::shutdownHardware()
{
    if (m_powered) {
        powerOff();
    }
}
SimulatedDeviceImpl::SimulatedDeviceImpl()
    : m_powered(false)
    , m_connected(false)
    , m_simulatedDelay(0)
    , m_failureRate(0.0f)
    , m_simulatedFailure(false)
{
}

SimulatedDeviceImpl::~SimulatedDeviceImpl()
{
}

void SimulatedDeviceImpl::powerOn()
{
    simulateDelay();

    if (shouldFail()) {
        Logger::getInstance().error("Simulated power on failure");
        return;
    }

    m_powered = true;
    Logger::getInstance().debug("Simulated device powered on");
}

void SimulatedDeviceImpl::powerOff()
{
    simulateDelay();

    if (shouldFail()) {
        Logger::getInstance().error("Simulated power off failure");
        return;
    }

    m_powered = false;
    Logger::getInstance().debug("Simulated device powered off");
}

bool SimulatedDeviceImpl::isPowered() const
{
    return m_powered;
}

int SimulatedDeviceImpl::getHardwareStatus() const
{
    if (m_simulatedFailure) {
        return HW_STATUS_ERROR;
    }
    return m_connected ? HW_STATUS_OK : HW_STATUS_DISCONNECTED;
}

std::string SimulatedDeviceImpl::getImplType() const
{
    return "Simulated";
}

std::string SimulatedDeviceImpl::getImplInfo() const
{
    std::ostringstream oss;
    oss << "Simulated Implementation [Delay: " << m_simulatedDelay
        << "ms, FailRate: " << (m_failureRate * 100) << "%]";
    return oss.str();
}

bool SimulatedDeviceImpl::isConnected() const
{
    return m_connected;
}

bool SimulatedDeviceImpl::connect()
{
    simulateDelay();
    m_connected = true;
    m_simulatedFailure = false;
    Logger::getInstance().debug("Simulated device connected");
    return true;
}

void SimulatedDeviceImpl::disconnect()
{
    m_connected = false;
    m_powered = false;
    Logger::getInstance().debug("Simulated device disconnected");
}

void SimulatedDeviceImpl::setSimulatedDelay(int milliseconds)
{
    if (milliseconds >= 0) {
        m_simulatedDelay = milliseconds;
    }
}

int SimulatedDeviceImpl::getSimulatedDelay() const
{
    return m_simulatedDelay;
}

void SimulatedDeviceImpl::setFailureRate(float rate)
{
    if (rate >= 0.0f && rate <= 1.0f) {
        m_failureRate = rate;
    }
}

float SimulatedDeviceImpl::getFailureRate() const
{
    return m_failureRate;
}

void SimulatedDeviceImpl::simulateFailure()
{
    m_simulatedFailure = true;
}

void SimulatedDeviceImpl::simulateDelay() const
{
    if (m_simulatedDelay > 0) {
    }
}

bool SimulatedDeviceImpl::shouldFail() const
{
    if (m_simulatedFailure) {
        return true;
    }

    if (m_failureRate > 0.0f) {
        float random = static_cast<float>(std::rand()) / static_cast<float>(RAND_MAX);
        return random < m_failureRate;
    }

    return false;
}
NetworkDeviceImpl::NetworkDeviceImpl(const std::string& endpoint)
    : m_endpoint(endpoint)
    , m_timeout(5000)
    , m_powered(false)
    , m_connected(false)
{
}

NetworkDeviceImpl::~NetworkDeviceImpl()
{
    if (m_connected) {
        disconnect();
    }
}

void NetworkDeviceImpl::powerOn()
{
    if (!m_connected) {
        Logger::getInstance().warning("Network device not connected");
        return;
    }

    if (sendCommand("POWER_ON")) {
        m_powered = true;
        Logger::getInstance().debug("Network device powered on: " + m_endpoint);
    }
}

void NetworkDeviceImpl::powerOff()
{
    if (!m_connected) {
        return;
    }

    if (sendCommand("POWER_OFF")) {
        m_powered = false;
        Logger::getInstance().debug("Network device powered off: " + m_endpoint);
    }
}

bool NetworkDeviceImpl::isPowered() const
{
    return m_powered;
}

int NetworkDeviceImpl::getHardwareStatus() const
{
    if (!m_connected) {
        return HW_STATUS_DISCONNECTED;
    }
    return HW_STATUS_OK;
}

std::string NetworkDeviceImpl::getImplType() const
{
    return "Network";
}

std::string NetworkDeviceImpl::getImplInfo() const
{
    std::ostringstream oss;
    oss << "Network Implementation [Endpoint: " << m_endpoint
        << ", Timeout: " << m_timeout << "ms]";
    return oss.str();
}

bool NetworkDeviceImpl::isConnected() const
{
    return m_connected;
}

bool NetworkDeviceImpl::connect()
{
    if (m_endpoint.empty()) {
        Logger::getInstance().error("No endpoint configured for network device");
        return false;
    }
    if (ping()) {
        m_connected = true;
        Logger::getInstance().info("Network device connected: " + m_endpoint);
        return true;
    }

    Logger::getInstance().error("Failed to connect to network device: " + m_endpoint);
    return false;
}

void NetworkDeviceImpl::disconnect()
{
    sendCommand("DISCONNECT");
    m_connected = false;
    m_powered = false;
    Logger::getInstance().info("Network device disconnected: " + m_endpoint);
}

void NetworkDeviceImpl::setEndpoint(const std::string& endpoint)
{
    m_endpoint = endpoint;
}

std::string NetworkDeviceImpl::getEndpoint() const
{
    return m_endpoint;
}

void NetworkDeviceImpl::setTimeout(int milliseconds)
{
    if (milliseconds > 0) {
        m_timeout = milliseconds;
    }
}

int NetworkDeviceImpl::getTimeout() const
{
    return m_timeout;
}

bool NetworkDeviceImpl::ping() const
{
    return !m_endpoint.empty();
}

bool NetworkDeviceImpl::sendCommand(const std::string& command)
{
    Logger::getInstance().debug("Sending command to " + m_endpoint + ": " + command);
    return true;
}

std::string NetworkDeviceImpl::receiveResponse()
{
    return "OK";
}
MockDeviceImpl::MockDeviceImpl()
    : m_powered(false)
    , m_connected(false)
    , m_status(HW_STATUS_OK)
    , m_powerOnCalls(0)
    , m_powerOffCalls(0)
    , m_connectCalls(0)
    , m_returnPowered(true)
    , m_returnConnected(true)
    , m_returnStatus(HW_STATUS_OK)
{
}

MockDeviceImpl::~MockDeviceImpl()
{
}

void MockDeviceImpl::powerOn()
{
    ++m_powerOnCalls;
    m_powered = m_returnPowered;
}

void MockDeviceImpl::powerOff()
{
    ++m_powerOffCalls;
    m_powered = false;
}

bool MockDeviceImpl::isPowered() const
{
    return m_powered;
}

int MockDeviceImpl::getHardwareStatus() const
{
    return m_returnStatus;
}

std::string MockDeviceImpl::getImplType() const
{
    return "Mock";
}

std::string MockDeviceImpl::getImplInfo() const
{
    return "Mock Implementation for Testing";
}

bool MockDeviceImpl::isConnected() const
{
    return m_connected;
}

bool MockDeviceImpl::connect()
{
    ++m_connectCalls;
    m_connected = m_returnConnected;
    return m_connected;
}

void MockDeviceImpl::disconnect()
{
    m_connected = false;
}

int MockDeviceImpl::getPowerOnCallCount() const
{
    return m_powerOnCalls;
}

int MockDeviceImpl::getPowerOffCallCount() const
{
    return m_powerOffCalls;
}

int MockDeviceImpl::getConnectCallCount() const
{
    return m_connectCalls;
}

void MockDeviceImpl::resetCallCounts()
{
    m_powerOnCalls = 0;
    m_powerOffCalls = 0;
    m_connectCalls = 0;
}

void MockDeviceImpl::setReturnPowered(bool powered)
{
    m_returnPowered = powered;
}

void MockDeviceImpl::setReturnConnected(bool connected)
{
    m_returnConnected = connected;
}

void MockDeviceImpl::setReturnStatus(int status)
{
    m_returnStatus = status;
}
IDeviceImpl* DeviceImplFactory::createImpl(ImplType type)
{
    switch (type) {
        case IMPL_HARDWARE:
            return new HardwareDeviceImpl();
        case IMPL_SIMULATED:
            return new SimulatedDeviceImpl();
        case IMPL_NETWORK:
            return new NetworkDeviceImpl();
        case IMPL_MOCK:
            return new MockDeviceImpl();
        default:
            return new SimulatedDeviceImpl();
    }
}

IDeviceImpl* DeviceImplFactory::createNetworkImpl(const std::string& endpoint)
{
    return new NetworkDeviceImpl(endpoint);
}

IDeviceImpl* DeviceImplFactory::createSimulatedImpl(int delayMs)
{
    SimulatedDeviceImpl* impl = new SimulatedDeviceImpl();
    impl->setSimulatedDelay(delayMs);
    return impl;
}

}
