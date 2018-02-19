/*
 * ForceController.h
 *
 *  Created on: Oct 23, 2017
 *      Author: ccontaxis
 */

#ifndef FORCECONTROLLER_H_
#define FORCECONTROLLER_H_

#include <DataTypes.h>
#include <ForceActuatorNeighbors.h>
#include <SAL_m1m3C.h>
#include <SAL_MTMountC.h>
#include <DistributedForces.h>
#include <PID.h>
#include <vector>

namespace LSST {
namespace M1M3 {
namespace SS {

class ForceActuatorApplicationSettings;
class ForceActuatorSettings;
class M1M3SSPublisher;
class SafetyController;
class PIDSettings;

class ForceController {
private:
	static double const sqrt2 = 1.4142135623730950488016887242097;
	static double const reciprocalSqrt2 = 0.70710678118654752440084436210485;

	PID fx;
	PID fy;
	PID fz;
	PID mx;
	PID my;
	PID mz;

	ForceActuatorApplicationSettings* forceActuatorApplicationSettings;
	ForceActuatorSettings* forceActuatorSettings;
	PIDSettings* pidSettings;
	M1M3SSPublisher* publisher;
	SafetyController* safetyController;

	m1m3_logevent_AppliedForcesC* appliedForces;
	m1m3_logevent_ForceActuatorInfoC* forceInfo;
	m1m3_ForceActuatorDataC* forceData;
	m1m3_logevent_ForceActuatorDataRejectionC* forceSetpoint;
	m1m3_logevent_ForceActuatorSetpointWarningC* forceSetpointWarning;
	m1m3_InclinometerDataC* inclinometerData;
	m1m3_PIDDataC* pidData;
	m1m3_logevent_PIDInfoC* pidInfo;
	m1m3_HardpointDataC* hardpointData;
	m1m3_MirrorForceDataC* mirrorForceData;
	m1m3_AccelerometerDataC* accelerometerData;
	m1m3_GyroDataC* gyroData;

	MTMount_AzC tmaAzimuthData;
	MTMount_AltC tmaElevationData;

	std::vector<ForceActuatorNeighbors> neighbors;

	static int32_t toInt24(double force) { return (int32_t)(force * 1000.0); }

public:
	ForceController(ForceActuatorApplicationSettings* forceActuatorApplicationSettings, ForceActuatorSettings* forceActuatorSettings, PIDSettings* pidSettings, M1M3SSPublisher* publisher, SafetyController* safetyController);

	void updateTMAAzimuthData(MTMount_AzC* tmaAzimuthData);
	void updateTMAElevationData(MTMount_AltC* tmaElevationData);

	void incSupportPercentage();
	void decSupportPercentage();
	void zeroSupportPercentage();
	void fillSupportPercentage();
	bool supportPercentageFilled();
	bool supportPercentageZeroed();

	void updateAppliedForces();
	void processAppliedForces();

	void applyStaticForces();
	void zeroStaticForces();

	void applyOffsetForces(double* x, double* y, double* z);
	void applyOffsetForces(float xForce, float yForce, float zForce, float xMoment, float yMoment, float zMoment);
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

	void applyHardpointCorrections();
	void zeroHardpointCorrections();
	void updatePID(int id, PIDParameters parameters);
	void resetPID(int id);
	void resetPIDs();
	void calculateMirrorForces();

private:
	DistributedForces calculateDistribution(float xForce, float yForce, float zForce, float xMoment, float yMoment, float zMoment);

	void updateElevationForces();
	void updateAzimuthForces();
	void updateTemperatureForces();
	void updateDynamicForces();
	void updateHardpointCorrectionForces();
	void sumAllForces();
	void convertForcesToSetpoints();

	bool checkMirrorMoments();
	bool checkNearNeighbors();
	bool checkMirrorWeight();
	bool checkFarNeighbors();

	PID* idToPID(int id);

	void publishAppliedForces();
	void publishForceSetpointWarning();
	void publishForceDataRejection();
};

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */

#endif /* FORCECONTROLLER_H_ */
