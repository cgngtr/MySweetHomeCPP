#include "DeviceIterator.h"
#include "Device.h"

namespace MySweetHome {
TypeFilter::TypeFilter(DeviceType type)
    : m_type(type)
{
}

TypeFilter::~TypeFilter()
{
}

bool TypeFilter::matches(const Device* device) const
{
    if (!device) return false;
    return device->getType() == m_type;
}
LocationFilter::LocationFilter(const std::string& location)
    : m_location(location)
{
}

LocationFilter::~LocationFilter()
{
}

bool LocationFilter::matches(const Device* device) const
{
    if (!device) return false;
    return device->getLocation() == m_location;
}
StatusFilter::StatusFilter(bool isOn)
    : m_isOn(isOn)
{
}

StatusFilter::~StatusFilter()
{
}

bool StatusFilter::matches(const Device* device) const
{
    if (!device) return false;
    return device->isOn() == m_isOn;
}
CriticalFilter::CriticalFilter(bool isCritical)
    : m_isCritical(isCritical)
{
}

CriticalFilter::~CriticalFilter()
{
}

bool CriticalFilter::matches(const Device* device) const
{
    if (!device) return false;
    return device->isCritical() == m_isCritical;
}
CompositeFilter::CompositeFilter()
    : m_ownsFilters(false)
{
}

CompositeFilter::~CompositeFilter()
{
    m_filters.clear();
}

void CompositeFilter::addFilter(IDeviceFilter* filter)
{
    if (filter) {
        m_filters.push_back(filter);
    }
}

void CompositeFilter::clearFilters()
{
    m_filters.clear();
}

bool CompositeFilter::matches(const Device* device) const
{
    if (!device) return false;
    for (size_t i = 0; i < m_filters.size(); ++i) {
        if (!m_filters[i]->matches(device)) {
            return false;
        }
    }
    return true;
}
FilteringDeviceIterator::FilteringDeviceIterator(
    const std::vector<Device*>& devices,
    IDeviceFilter* filter)
    : m_devices(devices)
    , m_filter(filter)
    , m_currentIndex(0)
{
    skipNonMatching();
}

FilteringDeviceIterator::~FilteringDeviceIterator()
{
}

void FilteringDeviceIterator::first()
{
    m_currentIndex = 0;
    skipNonMatching();
}

void FilteringDeviceIterator::next()
{
    if (!isDone()) {
        ++m_currentIndex;
        skipNonMatching();
    }
}

bool FilteringDeviceIterator::isDone() const
{
    return m_currentIndex >= m_devices.size();
}

Device* FilteringDeviceIterator::currentItem() const
{
    if (isDone()) {
        return 0;
    }
    return m_devices[m_currentIndex];
}

void FilteringDeviceIterator::setFilter(IDeviceFilter* filter)
{
    m_filter = filter;
    first();
}

size_t FilteringDeviceIterator::count() const
{
    if (!m_filter) {
        return m_devices.size();
    }

    size_t count = 0;
    for (size_t i = 0; i < m_devices.size(); ++i) {
        if (m_filter->matches(m_devices[i])) {
            ++count;
        }
    }
    return count;
}

void FilteringDeviceIterator::skipNonMatching()
{
    if (!m_filter) {
        return;
    }
    while (!isDone() && !m_filter->matches(m_devices[m_currentIndex])) {
        ++m_currentIndex;
    }
}
ReverseDeviceIterator::ReverseDeviceIterator(const std::vector<Device*>& devices)
    : m_devices(devices)
    , m_currentIndex(static_cast<int>(devices.size()) - 1)
{
}

ReverseDeviceIterator::~ReverseDeviceIterator()
{
}

void ReverseDeviceIterator::first()
{
    m_currentIndex = static_cast<int>(m_devices.size()) - 1;
}

void ReverseDeviceIterator::next()
{
    if (!isDone()) {
        --m_currentIndex;
    }
}

bool ReverseDeviceIterator::isDone() const
{
    return m_currentIndex < 0;
}

Device* ReverseDeviceIterator::currentItem() const
{
    if (isDone()) {
        return 0;
    }
    return m_devices[static_cast<size_t>(m_currentIndex)];
}

}
