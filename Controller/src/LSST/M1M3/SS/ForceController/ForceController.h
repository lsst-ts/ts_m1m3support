/*
 * ForceController.h
 *
 *  Created on: Oct 23, 2017
 *      Author: ccontaxis
 */

#ifndef FORCECONTROLLER_H_
#define FORCECONTROLLER_H_

#include <IForceController.h>
#include <DataTypes.h>
//#include <SAL_MTMountC.h>

struct m1m3_logevent_AppliedForcesC;
struct m1m3_logevent_ForceActuatorInfoC;
struct m1m3_ForceActuatorDataC;
struct m1m3_InclinometerDataC;

struct MTMount_AzC;
struct MTMount_AltC;

namespace LSST {
namespace M1M3 {
namespace SS {

class ForceActuatorApplicationSettings;
class ForceActuatorSettings;
class IPublisher;

class ForceController: public IForceController {
private:
	static double const sqrt2 = 1.4142135623730950488016887242097;
	static double const reciprocalSqrt2 = 0.70710678118654752440084436210485;

	ForceActuatorApplicationSettings* forceActuatorApplicationSettings;
	ForceActuatorSettings* forceActuatorSettings;
	IPublisher* publisher;

	m1m3_logevent_AppliedForcesC* appliedForces;
	m1m3_logevent_ForceActuatorInfoC* forceInfo;
	m1m3_ForceActuatorDataC* forceData;
	m1m3_InclinometerDataC* inclinometerData;

	//MTMount_AzC tmaAzimuthData;
	//MTMount_AltC tmaElevationData;

	double staticXSetpoint[156];
	double staticYSetpoint[156];
	double staticZSetpoint[156];

	static int32_t toInt24(double force) { return (int32_t)(force * 1000.0); }

public:
	ForceController(ForceActuatorApplicationSettings* forceActuatorApplicationSettings, ForceActuatorSettings* forceActuatorSettings, IPublisher* publisher);

	void updateTMAAzimuthData(MTMount_AzC* tmaAzimuthData);
	void updateTMAElevationData(MTMount_AltC* tmaElevationData);

	void updateAppliedForces();
	void processAppliedForces();

	void applyStaticForces();
	void zeroStaticForces();

	void applyOffsetForces(double* x, double* y, double* z);
	void zeroOffsetForces();

	void applyAOSCorrectionByBendingModes(double* coefficients);
	void applyAOSCorrectionByForces(double* z);
	void zeroAOSCorrection();

	void applyAberrationByBendingModes(double* coefficients);
	void applyAberrationByForces(double* z);
	void zeroAberration();

	void applyElevationForces();
	void zeroElevationForces();

	void applyAzimuthForces();
	void zeroAzimuthForces();

	void applyTemperatureForces();
	void zeroTemperatureForces();

	void applyDynamicForces();
	void zeroDynamicForces();

private:
	void updateElevationForces();
	void updateAzimuthForces();
	void updateTemperatureForces();
	void updateDynamicForces();
	void sumAllForces();
	void convertForcesToSetpoints();

	void publishAppliedForces();
};

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */

#endif /* FORCECONTROLLER_H_ */
