#include "DeviceCollection.h"
#include "Device.h"

namespace MySweetHome {
DeviceCollection::DeviceCollection()
{
}

DeviceCollection::~DeviceCollection()
{
    m_devices.clear();
}
void DeviceCollection::add(Device* device)
{
    if (device && !contains(device)) {
        m_devices.push_back(device);
    }
}

bool DeviceCollection::remove(Device* device)
{
    int index = findIndex(device);
    if (index >= 0) {
        m_devices.erase(m_devices.begin() + index);
        return true;
    }
    return false;
}

bool DeviceCollection::removeById(uint32_t id)
{
    int index = findIndexById(id);
    if (index >= 0) {
        m_devices.erase(m_devices.begin() + index);
        return true;
    }
    return false;
}

void DeviceCollection::clear()
{
    m_devices.clear();
}
bool DeviceCollection::contains(Device* device) const
{
    return findIndex(device) >= 0;
}

bool DeviceCollection::containsById(uint32_t id) const
{
    return findIndexById(id) >= 0;
}

Device* DeviceCollection::getById(uint32_t id) const
{
    int index = findIndexById(id);
    if (index >= 0) {
        return m_devices[static_cast<size_t>(index)];
    }
    return 0;
}

size_t DeviceCollection::size() const
{
    return m_devices.size();
}

bool DeviceCollection::isEmpty() const
{
    return m_devices.empty();
}

std::vector<Device*> DeviceCollection::toVector() const
{
    return m_devices;
}

Device* DeviceCollection::at(size_t index) const
{
    if (index < m_devices.size()) {
        return m_devices[index];
    }
    return 0;
}
IDeviceIterator* DeviceCollection::createIterator() const
{
    return new FilteringDeviceIterator(m_devices, 0);
}

IDeviceIterator* DeviceCollection::createFilteredIterator(IDeviceFilter* filter) const
{
    return new FilteringDeviceIterator(m_devices, filter);
}

IDeviceIterator* DeviceCollection::createTypeIterator(DeviceType type) const
{
    TypeFilter* filter = new TypeFilter(type);
    return new FilteringDeviceIterator(m_devices, filter);
}

IDeviceIterator* DeviceCollection::createLocationIterator(const std::string& location) const
{
    LocationFilter* filter = new LocationFilter(location);
    return new FilteringDeviceIterator(m_devices, filter);
}

IDeviceIterator* DeviceCollection::createActiveIterator() const
{
    StatusFilter* filter = new StatusFilter(true);
    return new FilteringDeviceIterator(m_devices, filter);
}

IDeviceIterator* DeviceCollection::createInactiveIterator() const
{
    StatusFilter* filter = new StatusFilter(false);
    return new FilteringDeviceIterator(m_devices, filter);
}

IDeviceIterator* DeviceCollection::createCriticalIterator() const
{
    CriticalFilter* filter = new CriticalFilter(true);
    return new FilteringDeviceIterator(m_devices, filter);
}

IDeviceIterator* DeviceCollection::createNonCriticalIterator() const
{
    CriticalFilter* filter = new CriticalFilter(false);
    return new FilteringDeviceIterator(m_devices, filter);
}

IDeviceIterator* DeviceCollection::createReverseIterator() const
{
    return new ReverseDeviceIterator(m_devices);
}
size_t DeviceCollection::countByType(DeviceType type) const
{
    TypeFilter filter(type);
    FilteringDeviceIterator it(m_devices, &filter);
    return it.count();
}

size_t DeviceCollection::countByLocation(const std::string& location) const
{
    LocationFilter filter(location);
    FilteringDeviceIterator it(m_devices, &filter);
    return it.count();
}

size_t DeviceCollection::countActive() const
{
    StatusFilter filter(true);
    FilteringDeviceIterator it(m_devices, &filter);
    return it.count();
}

size_t DeviceCollection::countCritical() const
{
    CriticalFilter filter(true);
    FilteringDeviceIterator it(m_devices, &filter);
    return it.count();
}
int DeviceCollection::findIndex(Device* device) const
{
    if (!device) return -1;

    for (size_t i = 0; i < m_devices.size(); ++i) {
        if (m_devices[i] == device) {
            return static_cast<int>(i);
        }
    }
    return -1;
}

int DeviceCollection::findIndexById(uint32_t id) const
{
    for (size_t i = 0; i < m_devices.size(); ++i) {
        if (m_devices[i] && m_devices[i]->getId() == id) {
            return static_cast<int>(i);
        }
    }
    return -1;
}

}
