#ifndef LSST_M1M3_SS_FORCECONTROLLER_AZIMUTHFORCECOMPONENT_H_
#define LSST_M1M3_SS_FORCECONTROLLER_AZIMUTHFORCECOMPONENT_H_

#include <ForceComponent.h>
#include <SAL_MTM1M3C.h>

namespace LSST {
namespace M1M3 {
namespace SS {

class M1M3SSPublisher;
class SafetyController;
class ForceActuatorApplicationSettings;
class ForceActuatorSettings;

class AzimuthForceComponent : public ForceComponent {
private:
    M1M3SSPublisher* publisher;
    SafetyController* safetyController;
    ForceActuatorApplicationSettings* forceActuatorApplicationSettings;
    ForceActuatorSettings* forceActuatorSettings;

    MTM1M3_logevent_forceActuatorStateC* forceActuatorState;
    MTM1M3_logevent_forceSetpointWarningC* forceSetpointWarning;
    MTM1M3_logevent_appliedAzimuthForcesC* appliedAzimuthForces;
    MTM1M3_logevent_rejectedAzimuthForcesC* rejectedAzimuthForces;

public:
    AzimuthForceComponent(M1M3SSPublisher* publisher, SafetyController* safetyController,
                          ForceActuatorApplicationSettings* forceActuatorApplicationSettings,
                          ForceActuatorSettings* forceActuatorSettings);

    void applyAzimuthForces(float* x, float* y, float* z);
    void applyAzimuthForcesByAzimuthAngle(float azimuthAngle);

protected:
    void postEnableDisableActions();
    void postUpdateActions();
};

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */

#endif /* LSST_M1M3_SS_FORCECONTROLLER_AZIMUTHFORCECOMPONENT_H_ */
