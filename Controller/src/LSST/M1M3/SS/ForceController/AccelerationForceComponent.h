/*
 * AccelerationForceComponent.h
 *
 *  Created on: Jul 9, 2018
 *      Author: ccontaxis
 */

#ifndef LSST_M1M3_SS_FORCECONTROLLER_ACCELERATIONFORCECOMPONENT_H_
#define LSST_M1M3_SS_FORCECONTROLLER_ACCELERATIONFORCECOMPONENT_H_

#include <ForceComponent.h>

struct MTM1M3_logevent_forceActuatorStateC;
struct MTM1M3_logevent_forceActuatorWarningC;
struct MTM1M3_logevent_appliedAccelerationForcesC;
struct MTM1M3_logevent_rejectedAccelerationForcesC;

namespace LSST {
namespace M1M3 {
namespace SS {

class M1M3SSPublisher;
class SafetyController;
class ForceActuatorApplicationSettings;
class ForceActuatorSettings;

class AccelerationForceComponent: public ForceComponent {
private:
	M1M3SSPublisher* publisher;
	SafetyController* safetyController;
	ForceActuatorApplicationSettings* forceActuatorApplicationSettings;
	ForceActuatorSettings* forceActuatorSettings;

	MTM1M3_logevent_forceActuatorStateC* forceActuatorState;
	MTM1M3_logevent_forceActuatorWarningC* forceActuatorWarning;
	MTM1M3_logevent_appliedAccelerationForcesC* appliedAccelerationForces;
	MTM1M3_logevent_rejectedAccelerationForcesC* rejectedAccelerationForces;

public:
	AccelerationForceComponent(M1M3SSPublisher* publisher, SafetyController* safetyController, ForceActuatorApplicationSettings* forceActuatorApplicationSettings, ForceActuatorSettings* forceActuatorSettings);

	void applyAccelerationForces(float* x, float* y, float* z);
	void applyAccelerationForcesByAngularAccelerations(float angularAccelerationX, float angularAccelerationY, float angularAccelerationZ);

protected:
	void postEnableDisableActions();
	void postUpdateActions();
};

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */

#endif /* LSST_M1M3_SS_FORCECONTROLLER_ACCELERATIONFORCECOMPONENT_H_ */
