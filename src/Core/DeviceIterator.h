#ifndef DEVICE_ITERATOR_H
#define DEVICE_ITERATOR_H

#include <vector>
#include <string>
#include "common_types.h"

namespace MySweetHome {

class Device;
class IDeviceIterator {
public:
    virtual ~IDeviceIterator() {}

    virtual void first() = 0;
    virtual void next() = 0;
    virtual bool isDone() const = 0;
    virtual Device* currentItem() const = 0;
};
class IDeviceFilter {
public:
    virtual ~IDeviceFilter() {}
    virtual bool matches(const Device* device) const = 0;
};
class TypeFilter : public IDeviceFilter {
public:
    TypeFilter(DeviceType type);
    virtual ~TypeFilter();
    virtual bool matches(const Device* device) const;

private:
    DeviceType m_type;
};
class LocationFilter : public IDeviceFilter {
public:
    LocationFilter(const std::string& location);
    virtual ~LocationFilter();
    virtual bool matches(const Device* device) const;

private:
    std::string m_location;
};
class StatusFilter : public IDeviceFilter {
public:
    StatusFilter(bool isOn);
    virtual ~StatusFilter();
    virtual bool matches(const Device* device) const;

private:
    bool m_isOn;
};
class CriticalFilter : public IDeviceFilter {
public:
    CriticalFilter(bool isCritical);
    virtual ~CriticalFilter();
    virtual bool matches(const Device* device) const;

private:
    bool m_isCritical;
};
class CompositeFilter : public IDeviceFilter {
public:
    CompositeFilter();
    virtual ~CompositeFilter();

    void addFilter(IDeviceFilter* filter);
    void clearFilters();
    virtual bool matches(const Device* device) const;

private:
    std::vector<IDeviceFilter*> m_filters;
    bool m_ownsFilters;
};
class FilteringDeviceIterator : public IDeviceIterator {
public:
    FilteringDeviceIterator(const std::vector<Device*>& devices,
                           IDeviceFilter* filter = 0);
    virtual ~FilteringDeviceIterator();

    virtual void first();
    virtual void next();
    virtual bool isDone() const;
    virtual Device* currentItem() const;
    void setFilter(IDeviceFilter* filter);
    size_t count() const;

private:
    void skipNonMatching();

    const std::vector<Device*>& m_devices;
    IDeviceFilter* m_filter;
    size_t m_currentIndex;
};
class ReverseDeviceIterator : public IDeviceIterator {
public:
    ReverseDeviceIterator(const std::vector<Device*>& devices);
    virtual ~ReverseDeviceIterator();

    virtual void first();
    virtual void next();
    virtual bool isDone() const;
    virtual Device* currentItem() const;

private:
    const std::vector<Device*>& m_devices;
    int m_currentIndex;
};

}

#endif
