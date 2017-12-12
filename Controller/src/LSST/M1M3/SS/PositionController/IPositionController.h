/*
 * IPositionController.h
 *
 *  Created on: Oct 30, 2017
 *      Author: ccontaxis
 */

#ifndef IPOSITIONCONTROLLER_H_
#define IPOSITIONCONTROLLER_H_

#include <DataTypes.h>

namespace LSST {
namespace M1M3 {
namespace SS {

// 0.0607 microns / step
// 2000 steps per second
// 121.4 microns / second
// 1.214 mm / 10 seconds
// 66 seconds for 8mm

class IPositionController {
public:
	virtual ~IPositionController();

	virtual double getRaiseLowerTimeout();

	virtual void enableChase(int32_t actuatorID);
	virtual void disableChase(int32_t actuatorID);
	virtual void enableChaseAll();
	virtual void disableChaseAll();

	virtual bool forcesInTolerance();
	virtual bool motionComplete();

	virtual void move(int32_t* steps);
	virtual void moveToEncoder(int32_t* encoderValues);
	virtual void moveToAbsolute(double x, double y, double z, double rX, double rY, double rZ);
	virtual void moveToReferencePosition();
	virtual void translate(double x, double y, double z, double rX, double rY, double rZ);
	virtual void stopMotion();

	virtual void updateSteps();
};

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */

#endif /* IPOSITIONCONTROLLER_H_ */
