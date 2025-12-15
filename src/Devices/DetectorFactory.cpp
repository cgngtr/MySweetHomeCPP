#include "DetectorFactory.h"

namespace MySweetHome {

StandardDetectorFactory::StandardDetectorFactory()
{
}

StandardDetectorFactory::~StandardDetectorFactory()
{
}

SmokeDetector* StandardDetectorFactory::createSmokeDetector(uint32_t id, const std::string& name, const std::string& location) {
    return new SmokeDetector(id, name, location);
}

GasDetector* StandardDetectorFactory::createGasDetector(uint32_t id, const std::string& name, const std::string& location) {
    return new GasDetector(id, name, location);
}

IDetectorFactory::DetectorPair StandardDetectorFactory::createDetectorPair(uint32_t baseId, const std::string& name, const std::string& location) {
    DetectorPair pair;
    pair.smoke = createSmokeDetector(baseId, name + " (Smoke)", location);
    pair.gas = createGasDetector(baseId + 1, name + " (Gas)", location);
    return pair;
}

}
