/*
 * DigitalInputOutput.h
 *
 *  Created on: Mar 29, 2018
 *      Author: ccontaxis
 */

#ifndef DIGITALINPUTOUTPUT_H_
#define DIGITALINPUTOUTPUT_H_

#include <DataTypes.h>
#include <IFPGA.h>

struct MTM1M3_logevent_airSupplyStatusC;
struct MTM1M3_logevent_airSupplyWarningC;
struct MTM1M3_logevent_cellLightStatusC;
struct MTM1M3_logevent_cellLightWarningC;
struct MTM1M3_logevent_interlockStatusC;
struct MTM1M3_logevent_interlockWarningC;

namespace LSST {
namespace M1M3 {
namespace SS {

class InterlockApplicationSettings;
struct SupportFPGAData;
class M1M3SSPublisher;
class SafetyController;

/*!
 * The class used to process digital inputs and outputs.
 */
class DigitalInputOutput {
private:
    InterlockApplicationSettings* interlockApplicationSettings;
    M1M3SSPublisher* publisher;
    SafetyController* safetyController;

    MTM1M3_logevent_airSupplyStatusC* airSupplyStatus;
    MTM1M3_logevent_airSupplyWarningC* airSupplyWarning;
    MTM1M3_logevent_cellLightStatusC* cellLightStatus;
    MTM1M3_logevent_cellLightWarningC* cellLightWarning;
    MTM1M3_logevent_interlockStatusC* interlockStatus;
    MTM1M3_logevent_interlockWarningC* interlockWarning;

    uint64_t lastDOTimestamp;
    uint64_t lastDITimestamp;

    double lastToggleTimestamp;

public:
    /*!
     * Instantiates the accelerometer.
     * @param[in] accelerometerSettings The accelerometer settings.
     * @param[in] publisher The publisher.
     */
    DigitalInputOutput(InterlockApplicationSettings* interlockApplicationSettings,
                       M1M3SSPublisher* publisher);

    /*!
     * Sets the safety controller.
     * @param[in] safetyController The safety controller.
     */
    void setSafetyController(SafetyController* safetyController);

    /*!
     * Processes currently available digital input and output data and publish it.
     */
    void processData();

    /*!
     * Toggles the heartbeat signal if a configured amount of time has passed.
     */
    void tryToggleHeartbeat();
    /*!
     * Turns the air on.
     */
    void turnAirOn();
    /*!
     * Turns the air off.
     */
    void turnAirOff();
    /*!
     * Turns the cell lights on.
     */
    void turnCellLightsOn();
    /*!
     * Turns the cell lights off.
     */
    void turnCellLightsOff();
};

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */

#endif /* DIGITALINPUTOUTPUT_H_ */
