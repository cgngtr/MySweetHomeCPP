#ifndef DEVICE_COLLECTION_H
#define DEVICE_COLLECTION_H

#include <vector>
#include <string>
#include "common_types.h"
#include "DeviceIterator.h"

namespace MySweetHome {

class Device;
class IDeviceCollection {
public:
    virtual ~IDeviceCollection() {}
    virtual IDeviceIterator* createIterator() const = 0;
    virtual IDeviceIterator* createFilteredIterator(IDeviceFilter* filter) const = 0;
};
class DeviceCollection : public IDeviceCollection {
public:
    DeviceCollection();
    virtual ~DeviceCollection();
    void add(Device* device);
    bool remove(Device* device);
    bool removeById(uint32_t id);
    void clear();
    bool contains(Device* device) const;
    bool containsById(uint32_t id) const;
    Device* getById(uint32_t id) const;
    size_t size() const;
    bool isEmpty() const;
    std::vector<Device*> toVector() const;
    Device* at(size_t index) const;
    virtual IDeviceIterator* createIterator() const;
    virtual IDeviceIterator* createFilteredIterator(IDeviceFilter* filter) const;
    IDeviceIterator* createTypeIterator(DeviceType type) const;
    IDeviceIterator* createLocationIterator(const std::string& location) const;
    IDeviceIterator* createActiveIterator() const;
    IDeviceIterator* createInactiveIterator() const;
    IDeviceIterator* createCriticalIterator() const;
    IDeviceIterator* createNonCriticalIterator() const;
    IDeviceIterator* createReverseIterator() const;
    size_t countByType(DeviceType type) const;
    size_t countByLocation(const std::string& location) const;
    size_t countActive() const;
    size_t countCritical() const;

private:
    std::vector<Device*> m_devices;
    int findIndex(Device* device) const;
    int findIndexById(uint32_t id) const;
};

}

#endif
