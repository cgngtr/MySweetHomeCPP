#ifndef DETECTOR_FACTORY_H
#define DETECTOR_FACTORY_H

#include "Detector.h"

namespace MySweetHome {
class IDetectorFactory {
public:
    virtual ~IDetectorFactory() {}

    virtual SmokeDetector* createSmokeDetector(uint32_t id, const std::string& name, const std::string& location) = 0;
    virtual GasDetector* createGasDetector(uint32_t id, const std::string& name, const std::string& location) = 0;
    struct DetectorPair {
        SmokeDetector* smoke;
        GasDetector* gas;
    };

    virtual DetectorPair createDetectorPair(uint32_t baseId, const std::string& name, const std::string& location) = 0;
};
class StandardDetectorFactory : public IDetectorFactory {
public:
    StandardDetectorFactory();
    virtual ~StandardDetectorFactory();

    virtual SmokeDetector* createSmokeDetector(uint32_t id, const std::string& name, const std::string& location);
    virtual GasDetector* createGasDetector(uint32_t id, const std::string& name, const std::string& location);
    virtual DetectorPair createDetectorPair(uint32_t baseId, const std::string& name, const std::string& location);
};

}

#endif
