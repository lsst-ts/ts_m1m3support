/*
 * IForceController.h
 *
 *  Created on: Oct 25, 2017
 *      Author: ccontaxis
 */

#ifndef IFORCECONTROLLER_H_
#define IFORCECONTROLLER_H_

struct MTMount_AzC;
struct MTMount_AltC;

namespace LSST {
namespace M1M3 {
namespace SS {

class IForceController {
public:
	virtual ~IForceController();

	virtual void updateTMAAzimuthData(MTMount_AzC* tmaAzimuthData);
	virtual void updateTMAElevationData(MTMount_AltC* tmaElevationData);

	virtual void updateAppliedForces();
	virtual void processAppliedForces();

	virtual void applyStaticForces();
	virtual void zeroStaticForces();

	virtual void applyOffsetForces(double* x, double* y, double* z);
	virtual void zeroOffsetForces();

	virtual void applyAOSCorrectionByBendingModes(double* coefficients);
	virtual void applyAOSCorrectionByForces(double* z);
	virtual void zeroAOSCorrection();

	virtual void applyAberrationByBendingModes(double* coefficients);
	virtual void applyAberrationByForces(double* z);
	virtual void zeroAberration();

	virtual void applyElevationForces();
	virtual void zeroElevationForces();

	virtual void applyAzimuthForces();
	virtual void zeroAzimuthForces();

	virtual void applyTemperatureForces();
	virtual void zeroTemperatureForces();

	virtual void applyDynamicForces();
	virtual void zeroDynamicForces();
};

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */

#endif /* IFORCECONTROLLER_H_ */
