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

	m1m3_logevent_AppliedAberrationForcesC* appliedAberrationForces;
	m1m3_logevent_AppliedAccelerationForcesC* appliedAccelerationForces;
	m1m3_logevent_AppliedActiveOpticForcesC* appliedActiveOpticForces;
	m1m3_logevent_AppliedAzimuthForcesC* appliedAzimuthForces;
	m1m3_logevent_AppliedBalanceForcesC* appliedBalanceForces;
	m1m3_logevent_AppliedElevationForcesC* appliedElevationForces;
	m1m3_logevent_AppliedForcesC* appliedForces;
	m1m3_logevent_AppliedOffsetForcesC* appliedOffsetForces;
	m1m3_logevent_AppliedStaticForcesC* appliedStaticForces;
	m1m3_logevent_AppliedThermalForcesC* appliedThermalForces;
	m1m3_logevent_AppliedVelocityForcesC* appliedVelocityForces;
	m1m3_logevent_ForceActuatorStateC* forceActuatorState;
	m1m3_logevent_ForceSetpointWarningC* forceSetpointWarning;
	m1m3_logevent_RejectedAberrationForcesC* rejectedAberrationForces;
	m1m3_logevent_RejectedAccelerationForcesC* rejectedAccelerationForces;
	m1m3_logevent_RejectedActiveOpticForcesC* rejectedActiveOpticForces;
	m1m3_logevent_RejectedAzimuthForcesC* rejectedAzimuthForces;
	m1m3_logevent_RejectedBalanceForcesC* rejectedBalanceForces;
	m1m3_logevent_RejectedCylinderForcesC* rejectedCylinderForces;
	m1m3_logevent_RejectedElevationForcesC* rejectedElevationForces;
	m1m3_logevent_RejectedForcesC* rejectedForces;
	m1m3_logevent_RejectedOffsetForcesC* rejectedOffsetForces;
	m1m3_logevent_RejectedStaticForcesC* rejectedStaticForces;
	m1m3_logevent_RejectedThermalForcesC* rejectedThermalForces;
	m1m3_logevent_RejectedVelocityForcesC* rejectedVelocityForces;

	m1m3_logevent_ForceActuatorInfoC* forceActuatorInfo;
	m1m3_ForceActuatorDataC* forceActuatorData;

	m1m3_InclinometerDataC* inclinometerData;
	m1m3_PIDDataC* pidData;
	m1m3_logevent_PIDInfoC* pidInfo;
	m1m3_HardpointActuatorDataC* hardpointActuatorData;
	m1m3_MeasuredMirrorForceDataC* measuredMirrorForceData;
	m1m3_HardpointMirrorForceDataC* hardpointMirrorForceData;
	m1m3_AccelerometerDataC* accelerometerData;
	m1m3_GyroDataC* gyroData;

	MTMount_AzC tmaAzimuthData;
	MTMount_AltC tmaElevationData;

	std::vector<ForceActuatorNeighbors> neighbors;

	static int32_t toInt24(float force) { return (int32_t)(force * 1000.0); }

public:
	ForceController(ForceActuatorApplicationSettings* forceActuatorApplicationSettings, ForceActuatorSettings* forceActuatorSettings, PIDSettings* pidSettings, M1M3SSPublisher* publisher, SafetyController* safetyController);

	void updateTMAElevationData(MTMount_AltC* tmaElevationData);

	void incSupportPercentage();
	void decSupportPercentage();
	void zeroSupportPercentage();
	void fillSupportPercentage();
	bool supportPercentageFilled();
	bool supportPercentageZeroed();

	void updateAppliedForces();
	void processAppliedForces();

	void applyAberrationForcesByBendingModes(float* coefficients);
	void applyAberrationForces(float* z);
	void zeroAberrationForces();

	void applyAccelerationForces();
	void updateAccelerationForces();
	void zeroAccelerationForces();

	void applyActiveOpticForcesByBendingModes(float* coefficients);
	void applyActiveOpticForces(float* z);
	void zeroActiveOpticForces();

	void applyAzimuthForces();
	void updateAzimuthForces(float azimuthAngle);
	void zeroAzimuthForces();

	void applyBalanceForces();
	void updateBalanceForces();
	void zeroBalanceForces();
	void calculateMirrorForces();
	void updatePID(int id, PIDParameters parameters);
	void resetPID(int id);
	void resetPIDs();

	void applyElevationForces();
	void updateElevationForces();
	void updateElevationForces(float elevation);
	void zeroElevationForces();

	void applyOffsetForces(float* x, float* y, float* z);
	void applyOffsetForcesByMirrorForces(float xForce, float yForce, float zForce, float xMoment, float yMoment, float zMoment);
	void zeroOffsetForces();

	void applyStaticForces();
	void zeroStaticForces();

	void applyThermalForces();
	void updateThermalForces();
	void zeroThermalForces();

	void applyVelocityForces();
	void updateVelocityForces();
	void zeroVelocityForces();

private:
	DistributedForces calculateDistribution(float xForce, float yForce, float zForce, float xMoment, float yMoment, float zMoment);

	void sumAllForces();
	void convertForcesToSetpoints();

	bool checkMirrorMoments();
	bool checkNearNeighbors();
	bool checkMirrorWeight();
	bool checkFarNeighbors();

	PID* idToPID(int id);
};

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */

#endif /* FORCECONTROLLER_H_ */
