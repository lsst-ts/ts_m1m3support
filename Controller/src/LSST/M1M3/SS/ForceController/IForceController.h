/*
 * IForceController.h
 *
 *  Created on: Oct 25, 2017
 *      Author: ccontaxis
 */

#ifndef IFORCECONTROLLER_H_
#define IFORCECONTROLLER_H_

namespace LSST {
namespace M1M3 {
namespace SS {

class IForceController {
public:
	virtual ~IForceController();

	virtual void updateAppliedForces();
	virtual void processAppliedForces();

	virtual void applyStaticForces();
	virtual void zeroStaticForces();

	virtual void applyOffsetForces(double* x, double* y, double* z);
	virtual void zeroOffsetForces();

	virtual void applyElevationForces();
	virtual void zeroElevationForces();
};

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */

#endif /* IFORCECONTROLLER_H_ */
