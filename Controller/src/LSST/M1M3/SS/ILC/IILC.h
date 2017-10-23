/*
 * IILC.h
 *
 *  Created on: Oct 3, 2017
 *      Author: ccontaxis
 */

#ifndef IILC_H_
#define IILC_H_

#include <DataTypes.h>

namespace LSST {
namespace M1M3 {
namespace SS {

/*!
 * An interface used for communicating with the ILCs (inner loop controllers).
 */
class IILC {
public:
	virtual ~IILC();

	/*!
	 * Sends the commands to update the ADC offsets and sensitivities to all ILCs.
	 */
	virtual void writeCalibrationDataBuffer();
	/*!
	 * Sends the command to update the ADC scan rate to all ILCs.
	 */
	virtual void writeSetADCScanRateBuffer();
	/*!
	 * Sends the command to update the boost valve DCA gains to all pneumatic ILCs.
	 */
	virtual void writeSetBoostValveDCAGainBuffer();
	/*!
	 * Sends the command to reset all ILCs.
	 */
	virtual void writeResetBuffer();
	/*!
	 * Sends the command to report server ID to all ILCs.
	 */
	virtual void writeReportServerIDBuffer();
	/*!
	 * Sends the command to report server status to all ILCs.
	 */
	virtual void writeReportServerStatusBuffer();
	/*!
	 * Sends the command to report ADC scan rate to all ILCs.
	 */
	virtual void writeReportADCScanRateBuffer();
	/*!
	 * Sends the command to read calibration to all ILCs.
	 */
	virtual void writeReadCalibrationDataBuffer();
	/*!
	 * Sends the command to read boost valve DCA gains to all pneumatic ILCs.
	 */
	virtual void writeReadBoostValveDCAGainBuffer();
	/*!
	 * Sends the command to report DCA ID to all pneumatic ILCs.
	 */
	virtual void writeReportDCAIDBuffer();
	/*!
	 * Sends the command to report DCA Status to all pneumatic ILCs.
	 */
	virtual void writeReportDCAStatusBuffer();
	/*!
	 * Sends the command to change the mode of all ILCs to standby.
	 */
	virtual void writeSetModeStandbyBuffer();
	/*!
	 * Sends the command to change the mode of all ILCs to disable.
	 */
	virtual void writeSetModeDisableBuffer();
	/*!
	 * Sends the command to change the mode of all ILCs to enable.
	 */
	virtual void writeSetModeEnableBuffer();
	/*!
	 * Sends the freeze sensor list to all ILCs.
	 * This sends the freeze sensor command and then queries the force and
	 * status of all ILCs.
	 * Electromechanical ILCs will have their server status queried.
	 * Pneumatic ILCs will have their server status queried in a round robin.
	 */
	virtual void writeFreezeSensorListBuffer();
	/*!
	 * Sends the raised list to all ILCs.
	 * This sends the broadcast force demand / broadcast step command and
	 * then queries the force and status of all ILCs.
	 * Electromechanical ILCs will have their server status queried.
	 * Pneumatic ILCs will have their server status queried in a round robin.
	 */
	virtual void writeRaisedListBuffer();
	/*!
	 * Sends a software trigger to the modbus subnets.
	 */
	virtual void triggerModbus();

	/*!
	 * Wait for a subnet to complete the execution of its instruction buffer.
	 * @param[in] subnet The subnet to read.
	 * @param[in] timeout The timeout in ms. -1 is infinite.
	 */
	virtual void waitForSubnet(int32_t subnet, int32_t timeout);
	/*!
	 * Wait for all subnets to complete their execution.
	 * @param[in] timeout The timeout in ms. -1 is infinite.
	 */
	virtual void waitForAllSubnets(int32_t timeout);

	/*!
	 * Read the Rx data for the specified subnet.
	 * @param[in] subnet The subnet to read.
	 */
	virtual void read(uint8_t subnet);
	/*!
	 * Read the Rx data from all subnets.
	 */
	virtual void readAll();

	/*!
	 * Checks for ILCs that haven't responded.
	 */
	virtual void verifyResponses();

	/*!
	 * Publishes force actuator status.
	 */
	virtual void publishForceActuatorStatus();

	/*!
	 * Publishes force actuator data.
	 */
	virtual void publishForceActuatorData();

	/*!
	 * Publishes hardpoint status.
	 */
	virtual void publishHardpointStatus();

	/*!
	 * Publishes hardpoint data.
	 */
	virtual void publishHardpointData();

	virtual void foo();
};

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */

#endif /* IILC_H_ */
