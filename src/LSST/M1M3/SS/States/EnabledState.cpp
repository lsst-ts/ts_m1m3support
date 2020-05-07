#include <EnabledState.h>
#include <Accelerometer.h>
#include <Displacement.h>
#include <ForceController.h>
#include <ILC.h>
#include <Inclinometer.h>
#include <DigitalInputOutput.h>
#include <Model.h>
#include <PositionController.h>
#include <SafetyController.h>
#include <PowerController.h>
#include <TMAAzimuthSampleCommand.h>
#include <TMAElevationSampleCommand.h>
#include <M1M3SSPublisher.h>
#include <Gyro.h>
#include <spdlog/spdlog.h>
#include <FPGA.h>
#include <SAL_MTM1M3C.h>

namespace LSST {
namespace M1M3 {
namespace SS {

EnabledState::EnabledState(M1M3SSPublisher* publisher) : State(publisher, "EnabledState") {}
EnabledState::EnabledState(M1M3SSPublisher* publisher, std::string name) : State(publisher, name) {}

States::Type EnabledState::update(UpdateCommand* command, Model* model) {
    spdlog::trace("EnabledState: update()");
    ILC* ilc = model->getILC();
    model->getForceController()->updateAppliedForces();
    model->getForceController()->processAppliedForces();
    ilc->writeControlListBuffer();
    ilc->triggerModbus();
    model->getDigitalInputOutput()->tryToggleHeartbeat();
    usleep(1000);
    IFPGA::get().pullTelemetry();
    model->getAccelerometer()->processData();
    model->getDigitalInputOutput()->processData();
    model->getDisplacement()->processData();
    model->getGyro()->processData();
    model->getInclinometer()->processData();
    model->getPowerController()->processData();
    ilc->waitForAllSubnets(5000);
    ilc->readAll();
    ilc->calculateHPPostion();
    ilc->calculateHPMirrorForces();
    ilc->calculateFAMirrorForces();
    ilc->verifyResponses();
    ilc->publishForceActuatorStatus();
    ilc->publishForceActuatorData();
    ilc->publishHardpointStatus();
    ilc->publishHardpointData();
    ilc->publishHardpointMonitorStatus();
    ilc->publishHardpointMonitorData();
    model->getPublisher()->tryLogHardpointActuatorWarning();
    return States::NoStateTransition;
}

States::Type EnabledState::storeTMAAzimuthSample(TMAAzimuthSampleCommand* command, Model* model) {
    spdlog::trace("EnabledState: storeTMAAzimuthSample()");
    model->getForceController()->updateAzimuthForces((float)command->getData()->Azimuth_Angle_Actual);
    return model->getSafetyController()->checkSafety(States::NoStateTransition);
}

States::Type EnabledState::storeTMAElevationSample(TMAElevationSampleCommand* command, Model* model) {
    spdlog::trace("EnabledState: storeTMAElevationSample()");
    model->getForceController()->updateTMAElevationData(command->getData());
    return model->getSafetyController()->checkSafety(States::NoStateTransition);
}

States::Type EnabledState::testAir(TestAirCommand* command, Model* model) {
    // TODO: Remove, this is a test command that has been taken for toggling boost valve control
    MTM1M3_logevent_forceActuatorStateC* forceActuatorState =
            model->getPublisher()->getEventForceActuatorState();
    MTM1M3_outerLoopDataC* outerLoop = model->getPublisher()->getOuterLoopData();
    spdlog::info("EnabledState: toggleBoostValve to {}", !forceActuatorState->slewFlag);
    forceActuatorState->slewFlag = !forceActuatorState->slewFlag;
    outerLoop->slewFlag = forceActuatorState->slewFlag;

    return model->getSafetyController()->checkSafety(States::NoStateTransition);
}

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */
