/*
 * ActiveOpticForceComponent.h
 *
 *  Created on: Jul 9, 2018
 *      Author: ccontaxis
 */

#ifndef LSST_M1M3_SS_FORCECONTROLLER_ACTIVEOPTICFORCECOMPONENT_H_
#define LSST_M1M3_SS_FORCECONTROLLER_ACTIVEOPTICFORCECOMPONENT_H_

#include <ForceComponent.h>

struct MTM1M3_logevent_forceActuatorStateC;
struct MTM1M3_logevent_forceActuatorWarningC;
struct MTM1M3_logevent_appliedActiveOpticForcesC;
struct MTM1M3_logevent_rejectedActiveOpticForcesC;

namespace LSST {
namespace M1M3 {
namespace SS {

class M1M3SSPublisher;
class SafetyController;
class ForceActuatorApplicationSettings;
class ForceActuatorSettings;

class ActiveOpticForceComponent : public ForceComponent {
private:
    M1M3SSPublisher* publisher;
    SafetyController* safetyController;
    ForceActuatorApplicationSettings* forceActuatorApplicationSettings;
    ForceActuatorSettings* forceActuatorSettings;

    MTM1M3_logevent_forceActuatorStateC* forceActuatorState;
    MTM1M3_logevent_forceActuatorWarningC* forceActuatorWarning;
    MTM1M3_logevent_appliedActiveOpticForcesC* appliedActiveOpticForces;
    MTM1M3_logevent_rejectedActiveOpticForcesC* rejectedActiveOpticForces;

public:
    ActiveOpticForceComponent(M1M3SSPublisher* publisher, SafetyController* safetyController, ForceActuatorApplicationSettings* forceActuatorApplicationSettings, ForceActuatorSettings* forceActuatorSettings);

    void applyActiveOpticForces(float* z);
    void applyActiveOpticForcesByBendingModes(float* coefficients);

protected:
    void postEnableDisableActions();
    void postUpdateActions();
};

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */

#endif /* LSST_M1M3_SS_FORCECONTROLLER_ACTIVEOPTICFORCECOMPONENT_H_ */
