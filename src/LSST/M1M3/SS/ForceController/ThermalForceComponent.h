#ifndef LSST_M1M3_SS_FORCECONTROLLER_THERMALFORCECOMPONENT_H_
#define LSST_M1M3_SS_FORCECONTROLLER_THERMALFORCECOMPONENT_H_

#include <ForceComponent.h>
#include <SAL_MTM1M3C.h>

namespace LSST {
namespace M1M3 {
namespace SS {

class M1M3SSPublisher;
class SafetyController;
class ForceActuatorApplicationSettings;
class ForceActuatorSettings;

class ThermalForceComponent : public ForceComponent {
private:
    M1M3SSPublisher* publisher;
    SafetyController* safetyController;
    ForceActuatorApplicationSettings* forceActuatorApplicationSettings;
    ForceActuatorSettings* forceActuatorSettings;

    MTM1M3_logevent_forceActuatorStateC* forceActuatorState;
    MTM1M3_logevent_forceSetpointWarningC* forceSetpointWarning;
    MTM1M3_logevent_appliedThermalForcesC* appliedThermalForces;
    MTM1M3_logevent_rejectedThermalForcesC* rejectedThermalForces;

public:
    ThermalForceComponent(M1M3SSPublisher* publisher, SafetyController* safetyController,
                          ForceActuatorApplicationSettings* forceActuatorApplicationSettings,
                          ForceActuatorSettings* forceActuatorSettings);

    void applyThermalForces(float* x, float* y, float* z);
    void applyThermalForcesByMirrorTemperature(float temperature);

protected:
    void postEnableDisableActions();
    void postUpdateActions();
};

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */

#endif /* LSST_M1M3_SS_FORCECONTROLLER_THERMALFORCECOMPONENT_H_ */
