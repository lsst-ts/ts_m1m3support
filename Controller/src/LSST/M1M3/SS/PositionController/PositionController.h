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

namespace LSST {
namespace M1M3 {
namespace SS {

class PositionControllerSettings;

class PositionController: public IPositionController {
private:
	PositionControllerSettings* positionControllerSettings;

	bool chasing[HP_COUNT];
	int32_t steps[HP_COUNT];

	m1m3_HardpointDataC* hardpointData;

public:
	PositionController(PositionControllerSettings* positionControllerSettings, m1m3_HardpointDataC* hardpointData);

	void enableChase(int32_t actuatorIndex);
	void disableChase(int32_t actuatorIndex);
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
