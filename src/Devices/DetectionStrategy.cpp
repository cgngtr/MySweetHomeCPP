#include "DetectionStrategy.h"
#include <cmath>

namespace MySweetHome {
SimpleThresholdStrategy::SimpleThresholdStrategy()
{
}

SimpleThresholdStrategy::~SimpleThresholdStrategy()
{
}

bool SimpleThresholdStrategy::detect(float sensorValue, float threshold) const
{
    return sensorValue > threshold;
}

std::string SimpleThresholdStrategy::getName() const
{
    return "SimpleThreshold";
}

std::string SimpleThresholdStrategy::getDescription() const
{
    return "Triggers when sensor value exceeds threshold";
}

float SimpleThresholdStrategy::getRecommendedThreshold() const
{
    return 50.0f;
}
AveragingDetectionStrategy::AveragingDetectionStrategy(int windowSize)
    : m_windowSize(windowSize > 0 ? windowSize : 5)
    , m_lastAverage(0.0f)
{
}

AveragingDetectionStrategy::~AveragingDetectionStrategy()
{
}

bool AveragingDetectionStrategy::detect(float sensorValue, float threshold) const
{
    AveragingDetectionStrategy* self = const_cast<AveragingDetectionStrategy*>(this);
    self->addReading(sensorValue);

    return m_lastAverage > threshold;
}

std::string AveragingDetectionStrategy::getName() const
{
    return "Averaging";
}

std::string AveragingDetectionStrategy::getDescription() const
{
    return "Uses moving average to reduce false positives";
}

float AveragingDetectionStrategy::getRecommendedThreshold() const
{
    return 45.0f;
}

void AveragingDetectionStrategy::addReading(float value)
{
    m_readings.push_back(value);
    while (static_cast<int>(m_readings.size()) > m_windowSize) {
        m_readings.erase(m_readings.begin());
    }
    m_lastAverage = getAverage();
}

float AveragingDetectionStrategy::getAverage() const
{
    if (m_readings.empty()) {
        return 0.0f;
    }

    float sum = 0.0f;
    for (size_t i = 0; i < m_readings.size(); ++i) {
        sum += m_readings[i];
    }
    return sum / static_cast<float>(m_readings.size());
}

void AveragingDetectionStrategy::clearReadings()
{
    m_readings.clear();
    m_lastAverage = 0.0f;
}
RateOfChangeStrategy::RateOfChangeStrategy(float rateThreshold)
    : m_previousValue(0.0f)
    , m_currentValue(0.0f)
    , m_rateThreshold(rateThreshold)
    , m_hasReadings(false)
{
}

RateOfChangeStrategy::~RateOfChangeStrategy()
{
}

bool RateOfChangeStrategy::detect(float sensorValue, float threshold) const
{
    RateOfChangeStrategy* self = const_cast<RateOfChangeStrategy*>(this);
    self->addReading(sensorValue);

    if (!m_hasReadings) {
        return false;
    }
    float rate = getRateOfChange();
    return (sensorValue > threshold) && (rate > m_rateThreshold);
}

std::string RateOfChangeStrategy::getName() const
{
    return "RateOfChange";
}

std::string RateOfChangeStrategy::getDescription() const
{
    return "Detects rapid increases in sensor values";
}

float RateOfChangeStrategy::getRecommendedThreshold() const
{
    return 40.0f;
}

void RateOfChangeStrategy::addReading(float value)
{
    m_previousValue = m_currentValue;
    m_currentValue = value;
    m_hasReadings = true;
}

float RateOfChangeStrategy::getRateOfChange() const
{
    if (!m_hasReadings) {
        return 0.0f;
    }
    return m_currentValue - m_previousValue;
}

void RateOfChangeStrategy::reset()
{
    m_previousValue = 0.0f;
    m_currentValue = 0.0f;
    m_hasReadings = false;
}
SmokeDetectionStrategy::SmokeDetectionStrategy()
    : m_sensitivity(5)
{
}

SmokeDetectionStrategy::~SmokeDetectionStrategy()
{
}

bool SmokeDetectionStrategy::detect(float sensorValue, float threshold) const
{
    float adjustedThreshold = calculateAdjustedThreshold(threshold);
    return sensorValue > adjustedThreshold;
}

std::string SmokeDetectionStrategy::getName() const
{
    return "SmokeDetection";
}

std::string SmokeDetectionStrategy::getDescription() const
{
    return "Optimized for smoke particle density detection";
}

float SmokeDetectionStrategy::getRecommendedThreshold() const
{
    return 3.0f;
}

void SmokeDetectionStrategy::setSensitivity(int level)
{
    if (level < 1) level = 1;
    if (level > 10) level = 10;
    m_sensitivity = level;
}

int SmokeDetectionStrategy::getSensitivity() const
{
    return m_sensitivity;
}

float SmokeDetectionStrategy::calculateAdjustedThreshold(float baseThreshold) const
{
    float factor = 1.5f - (static_cast<float>(m_sensitivity - 1) / 9.0f);
    return baseThreshold * factor;
}
GasDetectionStrategy::GasDetectionStrategy(const std::string& gasType)
    : m_gasType(gasType)
{
}

GasDetectionStrategy::~GasDetectionStrategy()
{
}

bool GasDetectionStrategy::detect(float sensorValue, float threshold) const
{
    float effectiveThreshold = threshold;
    if (threshold <= 0.0f) {
        effectiveThreshold = getDefaultThresholdForGas(m_gasType);
    }
    return sensorValue > effectiveThreshold;
}

std::string GasDetectionStrategy::getName() const
{
    return "GasDetection_" + m_gasType;
}

std::string GasDetectionStrategy::getDescription() const
{
    return "PPM-based detection for " + m_gasType + " gas";
}

float GasDetectionStrategy::getRecommendedThreshold() const
{
    return getDefaultThresholdForGas(m_gasType);
}

void GasDetectionStrategy::setGasType(const std::string& gasType)
{
    m_gasType = gasType;
}

std::string GasDetectionStrategy::getGasType() const
{
    return m_gasType;
}

float GasDetectionStrategy::getDefaultThresholdForGas(const std::string& gasType)
{
    if (gasType == "CO") {
        return 50.0f;
    } else if (gasType == "LPG") {
        return 1000.0f;
    } else if (gasType == "Methane") {
        return 5000.0f;
    } else if (gasType == "Natural") {
        return 2500.0f;
    }
    return 1000.0f;
}
MotionDetectionStrategy::MotionDetectionStrategy(int sensitivityLevel)
    : m_sensitivity(sensitivityLevel)
{
    if (m_sensitivity < 1) m_sensitivity = 1;
    if (m_sensitivity > 10) m_sensitivity = 10;
}

MotionDetectionStrategy::~MotionDetectionStrategy()
{
}

bool MotionDetectionStrategy::detect(float sensorValue, float threshold) const
{
    float factor = 1.0f + (static_cast<float>(10 - m_sensitivity) / 5.0f);
    float adjustedThreshold = threshold * factor;

    return sensorValue > adjustedThreshold;
}

std::string MotionDetectionStrategy::getName() const
{
    return "MotionDetection";
}

std::string MotionDetectionStrategy::getDescription() const
{
    return "PIR-based motion detection with adjustable sensitivity";
}

float MotionDetectionStrategy::getRecommendedThreshold() const
{
    return 30.0f;
}

void MotionDetectionStrategy::setSensitivity(int level)
{
    if (level < 1) level = 1;
    if (level > 10) level = 10;
    m_sensitivity = level;
}

int MotionDetectionStrategy::getSensitivity() const
{
    return m_sensitivity;
}
DetectionContext::DetectionContext()
    : m_strategy(0)
    , m_ownsStrategy(false)
{
}

DetectionContext::~DetectionContext()
{
    if (m_ownsStrategy && m_strategy) {
        delete m_strategy;
    }
}

void DetectionContext::setStrategy(IDetectionStrategy* strategy)
{
    if (m_ownsStrategy && m_strategy) {
        delete m_strategy;
    }
    m_strategy = strategy;
    m_ownsStrategy = false;
}

IDetectionStrategy* DetectionContext::getStrategy() const
{
    return m_strategy;
}

bool DetectionContext::executeDetection(float sensorValue, float threshold) const
{
    if (!m_strategy) {
        return sensorValue > threshold;
    }
    return m_strategy->detect(sensorValue, threshold);
}

std::string DetectionContext::getStrategyName() const
{
    if (!m_strategy) {
        return "None";
    }
    return m_strategy->getName();
}
IDetectionStrategy* DetectionStrategyFactory::createStrategy(StrategyType type)
{
    switch (type) {
        case STRATEGY_SIMPLE_THRESHOLD:
            return new SimpleThresholdStrategy();
        case STRATEGY_AVERAGING:
            return new AveragingDetectionStrategy();
        case STRATEGY_RATE_OF_CHANGE:
            return new RateOfChangeStrategy();
        case STRATEGY_SMOKE:
            return new SmokeDetectionStrategy();
        case STRATEGY_GAS:
            return new GasDetectionStrategy();
        case STRATEGY_MOTION:
            return new MotionDetectionStrategy();
        default:
            return new SimpleThresholdStrategy();
    }
}

IDetectionStrategy* DetectionStrategyFactory::createSmokeStrategy(int sensitivity)
{
    SmokeDetectionStrategy* strategy = new SmokeDetectionStrategy();
    strategy->setSensitivity(sensitivity);
    return strategy;
}

IDetectionStrategy* DetectionStrategyFactory::createGasStrategy(const std::string& gasType)
{
    return new GasDetectionStrategy(gasType);
}

IDetectionStrategy* DetectionStrategyFactory::createMotionStrategy(int sensitivity)
{
    return new MotionDetectionStrategy(sensitivity);
}

}
