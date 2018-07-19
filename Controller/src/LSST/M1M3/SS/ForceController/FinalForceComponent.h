/*
 * FinalForceComponent.h
 *
 *  Created on: Jul 10, 2018
 *      Author: ccontaxis
 */

#ifndef LSST_M1M3_SS_FORCECONTROLLER_FINALFORCECOMPONENT_H_
#define LSST_M1M3_SS_FORCECONTROLLER_FINALFORCECOMPONENT_H_

#include <ForceComponent.h>
#include <SAL_m1m3C.h>

namespace LSST {
namespace M1M3 {
namespace SS {

class M1M3SSPublisher;
class SafetyController;
class ForceActuatorApplicationSettings;
class ForceActuatorSettings;

class FinalForceComponent: public ForceComponent {
private:
	M1M3SSPublisher* publisher;
	SafetyController* safetyController;
	ForceActuatorApplicationSettings* forceActuatorApplicationSettings;
	ForceActuatorSettings* forceActuatorSettings;

	m1m3_logevent_ForceActuatorStateC* forceActuatorState;
	m1m3_logevent_ForceSetpointWarningC* forceSetpointWarning;
	m1m3_logevent_AppliedForcesC* appliedForces;
	m1m3_logevent_RejectedForcesC* rejectedForces;

	m1m3_logevent_AppliedAberrationForcesC* appliedAberrationForces;
	m1m3_logevent_AppliedAccelerationForcesC* appliedAccelerationForces;
	m1m3_logevent_AppliedActiveOpticForcesC* appliedActiveOpticForces;
	m1m3_logevent_AppliedAzimuthForcesC* appliedAzimuthForces;
	m1m3_logevent_AppliedBalanceForcesC* appliedBalanceForces;
	m1m3_logevent_AppliedElevationForcesC* appliedElevationForces;
	m1m3_logevent_AppliedOffsetForcesC* appliedOffsetForces;
	m1m3_logevent_AppliedStaticForcesC* appliedStaticForces;
	m1m3_logevent_AppliedThermalForcesC* appliedThermalForces;
	m1m3_logevent_AppliedVelocityForcesC* appliedVelocityForces;

public:
	FinalForceComponent(M1M3SSPublisher* publisher, SafetyController* safetyController, ForceActuatorApplicationSettings* forceActuatorApplicationSettings, ForceActuatorSettings* forceActuatorSettings);

	void applyForces(float* x, float* y, float* z);
	void applyForcesByComponents();

protected:
	void postEnableDisableActions();
	void postUpdateActions();
};

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */

#endif /* LSST_M1M3_SS_FORCECONTROLLER_FINALFORCECOMPONENT_H_ */
