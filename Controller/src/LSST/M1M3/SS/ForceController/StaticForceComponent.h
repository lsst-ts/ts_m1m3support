/*
 * StaticForceComponent.h
 *
 *  Created on: Jul 9, 2018
 *      Author: ccontaxis
 */

#ifndef LSST_M1M3_SS_FORCECONTROLLER_STATICFORCECOMPONENT_H_
#define LSST_M1M3_SS_FORCECONTROLLER_STATICFORCECOMPONENT_H_

#include <ForceComponent.h>
#include <SAL_m1m3C.h>
#include <vector>

namespace LSST {
namespace M1M3 {
namespace SS {

class M1M3SSPublisher;
class SafetyController;
class ForceActuatorApplicationSettings;
class ForceActuatorSettings;

class StaticForceComponent: public ForceComponent {
private:
	M1M3SSPublisher* publisher;
	SafetyController* safetyController;
	ForceActuatorApplicationSettings* forceActuatorApplicationSettings;
	ForceActuatorSettings* forceActuatorSettings;

	m1m3_logevent_ForceActuatorStateC* forceActuatorState;
	m1m3_logevent_ForceSetpointWarningC* forceSetpointWarning;
	m1m3_logevent_AppliedStaticForcesC* appliedStaticForces;
	m1m3_logevent_RejectedStaticForcesC* rejectedStaticForces;

public:
	StaticForceComponent(M1M3SSPublisher* publisher, SafetyController* safetyController, ForceActuatorApplicationSettings* forceActuatorApplicationSettings, ForceActuatorSettings* forceActuatorSettings);

	void applyStaticForces(std::vector<float>* x, std::vector<float>* y, std::vector<float>* z);

protected:
	void postEnableDisableActions();
	void postUpdateActions();
};

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */

#endif /* LSST_M1M3_SS_FORCECONTROLLER_STATICFORCECOMPONENT_H_ */
