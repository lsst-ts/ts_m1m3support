/*
 * VelocityForceComponent.h
 *
 *  Created on: Jul 9, 2018
 *      Author: ccontaxis
 */

#ifndef LSST_M1M3_SS_FORCECONTROLLER_VELOCITYFORCECOMPONENT_H_
#define LSST_M1M3_SS_FORCECONTROLLER_VELOCITYFORCECOMPONENT_H_

#include <ForceComponent.h>
#include <SAL_m1m3C.h>

namespace LSST {
namespace M1M3 {
namespace SS {

class M1M3SSPublisher;
class SafetyController;
class ForceActuatorApplicationSettings;
class ForceActuatorSettings;

class VelocityForceComponent: public ForceComponent {
private:
	M1M3SSPublisher* publisher;
	SafetyController* safetyController;
	ForceActuatorApplicationSettings* forceActuatorApplicationSettings;
	ForceActuatorSettings* forceActuatorSettings;

	m1m3_logevent_ForceActuatorStateC* forceActuatorState;
	m1m3_logevent_ForceSetpointWarningC* forceSetpointWarning;
	m1m3_logevent_AppliedVelocityForcesC* appliedVelocityForces;
	m1m3_logevent_RejectedVelocityForcesC* rejectedVelocityForces;

public:
	VelocityForceComponent(M1M3SSPublisher* publisher, SafetyController* safetyController, ForceActuatorApplicationSettings* forceActuatorApplicationSettings, ForceActuatorSettings* forceActuatorSettings);

	void applyVelocityForces(float* x, float* y, float* z);
	void applyVelocityForcesByAngularVelocity(float angularVelocityX, float angularVelocityY, float angularVelocityZ);

protected:
	void postEnableDisableActions();
	void postUpdateActions();
};

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */

#endif /* LSST_M1M3_SS_FORCECONTROLLER_VELOCITYFORCECOMPONENT_H_ */
