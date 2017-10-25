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

struct m1m3_logevent_ForceActuatorInfoC;
struct m1m3_ForceActuatorDataC;
struct m1m3_InclinometerDataC;

namespace LSST {
namespace M1M3 {
namespace SS {

class ForceActuatorApplicationSettings;
class ForceActuatorSettings;

class ForceController: public IForceController {
private:
	static double const sqrt2 = 1.4142135623730950488016887242097;
	static double const reciprocalSqrt2 = 0.70710678118654752440084436210485;

	ForceActuatorApplicationSettings* forceActuatorApplicationSettings;
	ForceActuatorSettings* forceActuatorSettings;
	m1m3_logevent_ForceActuatorInfoC* forceInfo;
	m1m3_ForceActuatorDataC* forceData;
	m1m3_InclinometerDataC* inclinometerData;

	bool applyingStaticForces;
	bool applyingOffsetForces;
	bool applyingElevationForces;

	double staticXSetpoint[156];
	double staticYSetpoint[156];
	double staticZSetpoint[156];

	static int32_t toInt24(double force) { return (int32_t)(force * 1000.0); }

public:
	ForceController(ForceActuatorApplicationSettings* forceActuatorApplicationSettings, ForceActuatorSettings* forceActuatorSettings, m1m3_logevent_ForceActuatorInfoC* forceInfo, m1m3_InclinometerDataC* inclinometerData, m1m3_ForceActuatorDataC* forceData);

	void updateAppliedForces();
	void processAppliedForces();

	void applyStaticForces();
	void zeroStaticForces();

	void applyOffsetForces(double* x, double* y, double* z);
	void zeroOffsetForces();

	void applyElevationForces();
	void zeroElevationForces();

	//void calculateAzimuthForces();
	//void calculateTemperatureForces();
	//void calculateHardpointForces();
	//void calculateDynamicForces();
	//void calculateAOSForces();
	//void calculateAberationForces();

private:
	void updateElevationForces();
	void sumAllForces();
	void convertForcesToSetpoints();
};

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */

#endif /* FORCECONTROLLER_H_ */
