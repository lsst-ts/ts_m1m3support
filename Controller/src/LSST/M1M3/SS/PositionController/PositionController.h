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

namespace LSST {
namespace M1M3 {
namespace SS {

class PositionControllerSettings;
class IPublisher;

class PositionController: public IPositionController {
private:
	PositionControllerSettings* positionControllerSettings;
	IPublisher* publisher;

	m1m3_HardpointDataC* hardpointData;
	m1m3_logevent_HardpointActuatorMotionStateC* hardpointActuatorMotionState;

public:
	PositionController(PositionControllerSettings* positionControllerSettings, IPublisher* publisher);

	void enableChase(int32_t actuatorID);
	void disableChase(int32_t actuatorID);
	void enableChaseAll();
	void disableChaseAll();

	void move(int32_t* steps);
	void translate(double x, double y, double z, double rX, double rY, double rZ);
	void stopMotion();

	void updateSteps();
};

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */

#endif /* POSITIONCONTROLLER_H_ */
