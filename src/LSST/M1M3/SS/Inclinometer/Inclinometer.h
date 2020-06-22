#ifndef INCLINOMETER_H_
#define INCLINOMETER_H_

#include <DataTypes.h>

struct MTM1M3_inclinometerDataC;
struct MTM1M3_logevent_inclinometerSensorWarningC;

namespace LSST {
namespace M1M3 {
namespace SS {

struct SupportFPGAData;
class M1M3SSPublisher;
class SafetyController;
class InclinometerSettings;

/*!
 * The class used to process inclinometer data.
 */
class Inclinometer {
private:
    SupportFPGAData* fpgaData;
    M1M3SSPublisher* publisher;
    SafetyController* safetyController;
    InclinometerSettings* inclinometerSettings;

    MTM1M3_inclinometerDataC* inclinometerData;
    MTM1M3_logevent_inclinometerSensorWarningC* inclinometerWarning;

    uint64_t lastSampleTimestamp;
    uint64_t lastErrorTimestamp;
    bool errorCleared;

public:
    /*!
     * Instantiates the inclinometer.
     * @param[in] fpgaData The fpga data.
     * @param[in] publisher The publisher.
     * @param[in] safetyController The safety controller.
     */
    Inclinometer(SupportFPGAData* fpgaData, M1M3SSPublisher* publisher, SafetyController* safetyController,
                 InclinometerSettings* inclinometerSettings);

    /*!
     * Processes currently available inclinometer data and publish it.
     */
    void processData();
};

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */

#endif /* INCLINOMETER_H_ */
