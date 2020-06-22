/*
 * OffsetForceComponent.h
 *
 *  Created on: Jul 6, 2018
 *      Author: ccontaxis
 */

#ifndef LSST_M1M3_SS_FORCECONTROLLER_OFFSETFORCECOMPONENT_H_
#define LSST_M1M3_SS_FORCECONTROLLER_OFFSETFORCECOMPONENT_H_

#include <ForceComponent.h>
#include <SAL_MTM1M3C.h>

namespace LSST {
namespace M1M3 {
namespace SS {

class M1M3SSPublisher;
class SafetyController;
class ForceActuatorApplicationSettings;
class ForceActuatorSettings;

class OffsetForceComponent : public ForceComponent {
private:
    M1M3SSPublisher* publisher;
    SafetyController* safetyController;
    ForceActuatorApplicationSettings* forceActuatorApplicationSettings;
    ForceActuatorSettings* forceActuatorSettings;

    MTM1M3_logevent_forceActuatorStateC* forceActuatorState;
    MTM1M3_logevent_forceSetpointWarningC* forceSetpointWarning;
    MTM1M3_logevent_appliedOffsetForcesC* appliedOffsetForces;
    MTM1M3_logevent_rejectedOffsetForcesC* rejectedOffsetForces;

public:
    OffsetForceComponent(M1M3SSPublisher* publisher, SafetyController* safetyController,
                         ForceActuatorApplicationSettings* forceActuatorApplicationSettings,
                         ForceActuatorSettings* forceActuatorSettings);

    void applyOffsetForces(float* x, float* y, float* z);
    void applyOffsetForcesByMirrorForces(float xForce, float yForce, float zForce, float xMoment,
                                         float yMoment, float zMoment);

protected:
    void postEnableDisableActions();
    void postUpdateActions();
};

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */

#endif /* LSST_M1M3_SS_FORCECONTROLLER_OFFSETFORCECOMPONENT_H_ */
