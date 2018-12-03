/*
 * PositionController.h
 *
 *  Created on: Oct 30, 2017
 *      Author: ccontaxis
 */

#ifndef POSITIONCONTROLLER_H_
#define POSITIONCONTROLLER_H_

#include <DataTypes.h>

struct MTM1M3_hardpointActuatorDataC;
struct MTM1M3_logevent_appliedHardpointStepsC;
struct MTM1M3_logevent_hardpointActuatorStateC;
struct MTM1M3_logevent_hardpointActuatorInfoC;

namespace LSST {
namespace M1M3 {
namespace SS {

class PositionControllerSettings;
class HardpointActuatorSettings;
class M1M3SSPublisher;

class PositionController {
private:
	PositionControllerSettings* positionControllerSettings;
	HardpointActuatorSettings* hardpointActuatorSettings;
	M1M3SSPublisher* publisher;

	MTM1M3_hardpointActuatorDataC* hardpointActuatorData;
	MTM1M3_logevent_appliedHardpointStepsC* appliedHardpointSteps;
	MTM1M3_logevent_hardpointActuatorStateC* hardpointActuatorState;
	MTM1M3_logevent_hardpointActuatorInfoC* hardpointInfo;

	int32_t scaledMaxStepsPerLoop[6];
	int32_t stableEncoderCount[6];

public:
	PositionController(PositionControllerSettings* positionControllerSettings, HardpointActuatorSettings* hardpointActuatorSettings, M1M3SSPublisher* publisher);

	double getRaiseLowerTimeout();

	bool enableChase(int32_t actuatorID);
	void disableChase(int32_t actuatorID);
	bool enableChaseAll();
	void disableChaseAll();

	bool forcesInTolerance();
	bool motionComplete();

	bool move(int32_t* steps);
	bool moveToEncoder(int32_t* encoderValues);
	bool moveToAbsolute(double x, double y, double z, double rX, double rY, double rZ);
	bool moveToReferencePosition();
	bool translate(double x, double y, double z, double rX, double rY, double rZ);
	void stopMotion();

	void updateSteps();

private:
	void convertToSteps(int32_t* steps, double x, double y, double z, double rX, double rY, double rZ);
	int32_t abs(int32_t x);
};

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */

#endif /* POSITIONCONTROLLER_H_ */
