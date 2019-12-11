/*
 * FaultState.cpp
 *
 *  Created on: Oct 26, 2017
 *      Author: ccontaxis
 */

#include <FaultState.h>
#include <DigitalInputOutput.h>
#include <Model.h>
#include <ILC.h>
#include <Displacement.h>
#include <Inclinometer.h>
#include <PowerController.h>
#include <SafetyController.h>
#include <ForceController.h>
#include <unistd.h>
#include <M1M3SSPublisher.h>
#include <Accelerometer.h>
#include <Log.h>
#include <Gyro.h>
#include <FPGA.h>

namespace LSST {
namespace M1M3 {
namespace SS {

FaultState::FaultState(M1M3SSPublisher* publisher) : State(publisher, "FaultState") {}
FaultState::FaultState(M1M3SSPublisher* publisher, std::string name) : State(publisher, name) {}

States::Type FaultState::update(UpdateCommand* command, Model* model) {
    Log.Trace("FaultState: update()");
    this->startTimer();
    ILC* ilc = model->getILC();
    ilc->writeFreezeSensorListBuffer();
    ilc->triggerModbus();
    model->getDigitalInputOutput()->tryToggleHeartbeat();
    usleep(1000);
    model->getFPGA()->pullTelemetry();
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
    this->stopTimer();
    model->publishOuterLoop(this->getTimer());
    return States::NoStateTransition;
}

States::Type FaultState::standby(StandbyCommand* command, Model* model) {
    Log.Trace("FaultState: standby()");
    States::Type newState = States::StandbyState;
    model->getILC()->writeSetModeStandbyBuffer();
    model->getILC()->triggerModbus();
    model->getILC()->waitForAllSubnets(5000);
    model->getILC()->readAll();
    model->getILC()->verifyResponses();
    model->getPowerController()->setAllPowerNetworks(false);
    model->getSafetyController()->clearErrorCode();
    return newState;
}

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */
