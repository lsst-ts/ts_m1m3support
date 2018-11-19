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
#include <ForcesAndMoments.h>
#include <SAL_MTMountC.h>
#include <DistributedForces.h>
#include <PID.h>
#include <vector>
#include <AberrationForceComponent.h>
#include <AccelerationForceComponent.h>
#include <ActiveOpticForceComponent.h>
#include <AzimuthForceComponent.h>
#include <BalanceForceComponent.h>
#include <ElevationForceComponent.h>
#include <OffsetForceComponent.h>
#include <StaticForceComponent.h>
#include <ThermalForceComponent.h>
#include <VelocityForceComponent.h>
#include <FinalForceComponent.h>

struct MTM1M3_logevent_appliedCylinderForcesC;
struct MTM1M3_logevent_appliedForcesC;
struct MTM1M3_logevent_forceActuatorStateC;
struct MTM1M3_logevent_forceActuatorWarningC;
struct MTM1M3_logevent_rejectedCylinderForcesC;
struct MTM1M3_logevent_rejectedForcesC;
struct MTM1M3_logevent_forceActuatorPositionInfoC;
struct MTM1M3_forceActuatorDataC;
struct MTM1M3_inclinometerDataC;
struct MTM1M3_pidDataC;
struct MTM1M3_logevent_pidInfoC;
struct MTM1M3_hardpointActuatorDataC;
struct MTM1M3_accelerometerDataC;
struct MTM1M3_gyroDataC;

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

	ForceActuatorApplicationSettings* forceActuatorApplicationSettings;
	ForceActuatorSettings* forceActuatorSettings;
	PIDSettings* pidSettings;
	M1M3SSPublisher* publisher;
	SafetyController* safetyController;

	AberrationForceComponent aberrationForceComponent;
	AccelerationForceComponent accelerationForceComponent;
	ActiveOpticForceComponent activeOpticForceComponent;
	AzimuthForceComponent azimuthForceComponent;
	BalanceForceComponent balanceForceComponent;
	ElevationForceComponent elevationForceComponent;
	OffsetForceComponent offsetForceComponent;
	StaticForceComponent staticForceComponent;
	ThermalForceComponent thermalForceComponent;
	VelocityForceComponent velocityForceComponent;
	FinalForceComponent finalForceComponent;

	MTM1M3_logevent_appliedCylinderForcesC* appliedCylinderForces;
	MTM1M3_logevent_appliedForcesC* appliedForces;
	MTM1M3_logevent_forceActuatorStateC* forceActuatorState;
	MTM1M3_logevent_forceActuatorWarningC* forceActuatorWarning;
	MTM1M3_logevent_rejectedCylinderForcesC* rejectedCylinderForces;
	MTM1M3_logevent_rejectedForcesC* rejectedForces;

	MTM1M3_logevent_forceActuatorPositionInfoC* forceActuatorPositionInfo;
	MTM1M3_forceActuatorDataC* forceActuatorData;

	MTM1M3_inclinometerDataC* inclinometerData;
	MTM1M3_pidDataC* pidData;
	MTM1M3_logevent_pidInfoC* pidInfo;
	MTM1M3_hardpointActuatorDataC* hardpointActuatorData;
	MTM1M3_accelerometerDataC* accelerometerData;
	MTM1M3_gyroDataC* gyroData;

	MTMount_AzimuthC* tmaAzimuthData;
	MTMount_ElevationC* tmaElevationData;

	std::vector<ForceActuatorNeighbors> neighbors;

	float zero[156];
	float mirrorWeight;

	static int32_t toInt24(float force) { return (int32_t)(force * 1000.0); }

public:
	ForceController(ForceActuatorApplicationSettings* forceActuatorApplicationSettings, ForceActuatorSettings* forceActuatorSettings, PIDSettings* pidSettings, M1M3SSPublisher* publisher, SafetyController* safetyController, MTMount_ElevationC* tmaAlt, MTMount_AzimuthC* tmaAz);

	void reset();

	void updateTMAElevationData(MTMount_ElevationC* tmaElevationData);

	void incSupportPercentage();
	void decSupportPercentage();
	void zeroSupportPercentage();
	void fillSupportPercentage();
	bool supportPercentageFilled();
	bool supportPercentageZeroed();
	bool followingErrorInTolerance();

	void updateAppliedForces();
	void processAppliedForces();

	void applyAberrationForcesByBendingModes(float* coefficients);
	void applyAberrationForces(float* z);
	void zeroAberrationForces();

	void applyAccelerationForces();
	void zeroAccelerationForces();

	void applyActiveOpticForcesByBendingModes(float* coefficients);
	void applyActiveOpticForces(float* z);
	void zeroActiveOpticForces();

	void applyAzimuthForces();
	void updateAzimuthForces(float azimuthAngle);
	void zeroAzimuthForces();

	void applyBalanceForces();
	void zeroBalanceForces();
	void updatePID(int id, PIDParameters parameters);
	void resetPID(int id);
	void resetPIDs();

	void applyElevationForces();
	void zeroElevationForces();

	void applyOffsetForces(float* x, float* y, float* z);
	void applyOffsetForcesByMirrorForces(float xForce, float yForce, float zForce, float xMoment, float yMoment, float zMoment);
	void zeroOffsetForces();

	void applyStaticForces();
	void zeroStaticForces();

	void applyThermalForces();
	void updateThermalForces(float temperature);
	void zeroThermalForces();

	void applyVelocityForces();
	void zeroVelocityForces();

private:
	void sumAllForces();
	void convertForcesToSetpoints();

	bool checkMirrorMoments();
	bool checkNearNeighbors();
	bool checkMirrorWeight();
	bool checkFarNeighbors();
};

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */

#endif /* FORCECONTROLLER_H_ */
