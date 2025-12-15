#ifndef IDETECTION_STRATEGY_H
#define IDETECTION_STRATEGY_H

#include <string>

namespace MySweetHome {
class IDetectionStrategy {
public:
    virtual ~IDetectionStrategy() {}
    virtual bool detect(float sensorValue, float threshold) const = 0;
    virtual std::string getName() const = 0;
    virtual std::string getDescription() const = 0;
    virtual float getRecommendedThreshold() const = 0;
};

}

#endif
