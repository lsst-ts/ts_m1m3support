/*
 * PositionController.h
 *
 *  Created on: Oct 30, 2017
 *      Author: ccontaxis
 */

#ifndef POSITIONCONTROLLER_H_
#define POSITIONCONTROLLER_H_

#include <IPositionController.h>
#include <DataTypes.h>

struct m1m3_HardpointDataC;
struct m1m3_logevent_HardpointActuatorMotionStateC;
struct m1m3_logevent_HardpointActuatorInfoC;

namespace LSST {
namespace M1M3 {
namespace SS {

class PositionControllerSettings;
class HardpointActuatorSettings;
class IPublisher;

class PositionController: public IPositionController {
private:
	PositionControllerSettings* positionControllerSettings;
	HardpointActuatorSettings* hardpointActuatorSettings;
	IPublisher* publisher;

	m1m3_HardpointDataC* hardpointData;
	m1m3_logevent_HardpointActuatorMotionStateC* hardpointActuatorMotionState;
	m1m3_logevent_HardpointActuatorInfoC* hardpointInfo;

	int32_t scaledMaxStepsPerLoop[6];

public:
	PositionController(PositionControllerSettings* positionControllerSettings, HardpointActuatorSettings* hardpointActuatorSettings, IPublisher* publisher);

	double getRaiseLowerTimeout();

	void enableChase(int32_t actuatorID);
	void disableChase(int32_t actuatorID);
	void enableChaseAll();
	void disableChaseAll();

	bool forcesInTolerance();
	bool motionComplete();

	void move(int32_t* steps);
	void moveToEncoder(int32_t* encoderValues);
	void moveToAbsolute(double x, double y, double z, double rX, double rY, double rZ);
	void moveToReferencePosition();
	void translate(double x, double y, double z, double rX, double rY, double rZ);
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
