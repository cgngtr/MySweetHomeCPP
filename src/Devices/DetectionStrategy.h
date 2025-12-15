#ifndef DETECTION_STRATEGY_H
#define DETECTION_STRATEGY_H

#include "IDetectionStrategy.h"
#include <vector>
#include <string>

namespace MySweetHome {
class SimpleThresholdStrategy : public IDetectionStrategy {
public:
    SimpleThresholdStrategy();
    virtual ~SimpleThresholdStrategy();

    virtual bool detect(float sensorValue, float threshold) const;
    virtual std::string getName() const;
    virtual std::string getDescription() const;
    virtual float getRecommendedThreshold() const;
};
class AveragingDetectionStrategy : public IDetectionStrategy {
public:
    AveragingDetectionStrategy(int windowSize = 5);
    virtual ~AveragingDetectionStrategy();

    virtual bool detect(float sensorValue, float threshold) const;
    virtual std::string getName() const;
    virtual std::string getDescription() const;
    virtual float getRecommendedThreshold() const;
    void addReading(float value);
    float getAverage() const;
    void clearReadings();

private:
    std::vector<float> m_readings;
    int m_windowSize;
    mutable float m_lastAverage;
};
class RateOfChangeStrategy : public IDetectionStrategy {
public:
    RateOfChangeStrategy(float rateThreshold = 10.0f);
    virtual ~RateOfChangeStrategy();

    virtual bool detect(float sensorValue, float threshold) const;
    virtual std::string getName() const;
    virtual std::string getDescription() const;
    virtual float getRecommendedThreshold() const;
    void addReading(float value);
    float getRateOfChange() const;
    void reset();

private:
    float m_previousValue;
    float m_currentValue;
    float m_rateThreshold;
    bool m_hasReadings;
};
class SmokeDetectionStrategy : public IDetectionStrategy {
public:
    SmokeDetectionStrategy();
    virtual ~SmokeDetectionStrategy();

    virtual bool detect(float sensorValue, float threshold) const;
    virtual std::string getName() const;
    virtual std::string getDescription() const;
    virtual float getRecommendedThreshold() const;
    void setSensitivity(int level);
    int getSensitivity() const;

private:
    int m_sensitivity;
    float calculateAdjustedThreshold(float baseThreshold) const;
};
class GasDetectionStrategy : public IDetectionStrategy {
public:
    GasDetectionStrategy(const std::string& gasType = "LPG");
    virtual ~GasDetectionStrategy();

    virtual bool detect(float sensorValue, float threshold) const;
    virtual std::string getName() const;
    virtual std::string getDescription() const;
    virtual float getRecommendedThreshold() const;
    void setGasType(const std::string& gasType);
    std::string getGasType() const;
    static float getDefaultThresholdForGas(const std::string& gasType);

private:
    std::string m_gasType;
};
class MotionDetectionStrategy : public IDetectionStrategy {
public:
    MotionDetectionStrategy(int sensitivityLevel = 5);
    virtual ~MotionDetectionStrategy();

    virtual bool detect(float sensorValue, float threshold) const;
    virtual std::string getName() const;
    virtual std::string getDescription() const;
    virtual float getRecommendedThreshold() const;
    void setSensitivity(int level);
    int getSensitivity() const;

private:
    int m_sensitivity;
};
class DetectionContext {
public:
    DetectionContext();
    ~DetectionContext();

    void setStrategy(IDetectionStrategy* strategy);
    IDetectionStrategy* getStrategy() const;

    bool executeDetection(float sensorValue, float threshold) const;
    std::string getStrategyName() const;

private:
    IDetectionStrategy* m_strategy;
    bool m_ownsStrategy;
};
class DetectionStrategyFactory {
public:
    enum StrategyType {
        STRATEGY_SIMPLE_THRESHOLD,
        STRATEGY_AVERAGING,
        STRATEGY_RATE_OF_CHANGE,
        STRATEGY_SMOKE,
        STRATEGY_GAS,
        STRATEGY_MOTION
    };

    static IDetectionStrategy* createStrategy(StrategyType type);
    static IDetectionStrategy* createSmokeStrategy(int sensitivity = 5);
    static IDetectionStrategy* createGasStrategy(const std::string& gasType = "LPG");
    static IDetectionStrategy* createMotionStrategy(int sensitivity = 5);
};

}

#endif
