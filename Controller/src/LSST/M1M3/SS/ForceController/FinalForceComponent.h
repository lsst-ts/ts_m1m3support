/*
 * FinalForceComponent.h
 *
 *  Created on: Jul 10, 2018
 *      Author: ccontaxis
 */

#ifndef LSST_M1M3_SS_FORCECONTROLLER_FINALFORCECOMPONENT_H_
#define LSST_M1M3_SS_FORCECONTROLLER_FINALFORCECOMPONENT_H_

#include <ForceComponent.h>

struct MTM1M3_logevent_forceActuatorStateC;
struct MTM1M3_logevent_forceActuatorWarningC;
struct MTM1M3_logevent_appliedForcesC;
struct MTM1M3_logevent_rejectedForcesC;
struct MTM1M3_logevent_appliedAberrationForcesC;
struct MTM1M3_logevent_appliedAccelerationForcesC;
struct MTM1M3_logevent_appliedActiveOpticForcesC;
struct MTM1M3_logevent_appliedAzimuthForcesC;
struct MTM1M3_logevent_appliedBalanceForcesC;
struct MTM1M3_logevent_appliedElevationForcesC;
struct MTM1M3_logevent_appliedOffsetForcesC;
struct MTM1M3_logevent_appliedStaticForcesC;
struct MTM1M3_logevent_appliedThermalForcesC;
struct MTM1M3_logevent_appliedVelocityForcesC;

namespace LSST {
namespace M1M3 {
namespace SS {

class M1M3SSPublisher;
class SafetyController;
class ForceActuatorApplicationSettings;
class ForceActuatorSettings;

class FinalForceComponent : public ForceComponent {
   private:
    M1M3SSPublisher* publisher;
    SafetyController* safetyController;
    ForceActuatorApplicationSettings* forceActuatorApplicationSettings;
    ForceActuatorSettings* forceActuatorSettings;

    MTM1M3_logevent_forceActuatorStateC* forceActuatorState;
    MTM1M3_logevent_forceActuatorWarningC* forceActuatorWarning;
    MTM1M3_logevent_appliedForcesC* appliedForces;
    MTM1M3_logevent_rejectedForcesC* rejectedForces;

    MTM1M3_logevent_appliedAberrationForcesC* appliedAberrationForces;
    MTM1M3_logevent_appliedAccelerationForcesC* appliedAccelerationForces;
    MTM1M3_logevent_appliedActiveOpticForcesC* appliedActiveOpticForces;
    MTM1M3_logevent_appliedAzimuthForcesC* appliedAzimuthForces;
    MTM1M3_logevent_appliedBalanceForcesC* appliedBalanceForces;
    MTM1M3_logevent_appliedElevationForcesC* appliedElevationForces;
    MTM1M3_logevent_appliedOffsetForcesC* appliedOffsetForces;
    MTM1M3_logevent_appliedStaticForcesC* appliedStaticForces;
    MTM1M3_logevent_appliedThermalForcesC* appliedThermalForces;
    MTM1M3_logevent_appliedVelocityForcesC* appliedVelocityForces;

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
