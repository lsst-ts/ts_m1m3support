/*
 * StaticForceComponent.h
 *
 *  Created on: Jul 9, 2018
 *      Author: ccontaxis
 */

#ifndef LSST_M1M3_SS_FORCECONTROLLER_STATICFORCECOMPONENT_H_
#define LSST_M1M3_SS_FORCECONTROLLER_STATICFORCECOMPONENT_H_

#include <ForceComponent.h>
#include <vector>

struct MTM1M3_logevent_forceActuatorStateC;
struct MTM1M3_logevent_forceActuatorWarningC;
struct MTM1M3_logevent_appliedStaticForcesC;
struct MTM1M3_logevent_rejectedStaticForcesC;

namespace LSST {
namespace M1M3 {
namespace SS {

class M1M3SSPublisher;
class SafetyController;
class ForceActuatorApplicationSettings;
class ForceActuatorSettings;

class StaticForceComponent : public ForceComponent {
   private:
    M1M3SSPublisher* publisher;
    SafetyController* safetyController;
    ForceActuatorApplicationSettings* forceActuatorApplicationSettings;
    ForceActuatorSettings* forceActuatorSettings;

    MTM1M3_logevent_forceActuatorStateC* forceActuatorState;
    MTM1M3_logevent_forceActuatorWarningC* forceActuatorWarning;
    MTM1M3_logevent_appliedStaticForcesC* appliedStaticForces;
    MTM1M3_logevent_rejectedStaticForcesC* rejectedStaticForces;

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
