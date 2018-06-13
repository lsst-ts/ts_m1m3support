/*
 * DisabledState.cpp
 *
 *  Created on: Sep 27, 2017
 *      Author: ccontaxis
 */

#include <DisabledState.h>
#include <Accelerometer.h>
#include <Displacement.h>
#include <ILC.h>
#include <Inclinometer.h>
#include <DigitalInputOutput.h>
#include <Model.h>
#include <SafetyController.h>
#include <PowerController.h>
#include <Gyro.h>
#include <ForceController.h>
#include <M1M3SSPublisher.h>
#include <ProgramILCCommand.h>
#include <ModbusTransmitCommand.h>
#include <Log.h>
#include <unistd.h>
#include <FPGA.h>

namespace LSST {
namespace M1M3 {
namespace SS {

DisabledState::DisabledState(M1M3SSPublisher* publisher) : State(publisher, "DisabledState") { }

States::Type DisabledState::update(UpdateCommand* command, Model* model) {
	Log.Trace("DisabledState::update()");
	this->startTimer();
	ILC* ilc = model->getILC();
	ilc->writeFreezeSensorListBuffer();
	ilc->triggerModbus();
	model->getDigitalInputOutput()->tryToggleHeartbeat();
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
	model->getPublisher()->tryLogHardpointActuatorWarning();
	this->stopTimer();
	model->publishOuterLoop(this->getTimer());
	return model->getSafetyController()->checkSafety(States::NoStateTransition);
}

States::Type DisabledState::enable(EnableCommand* command, Model* model) {
	Log.Info("DisabledState: enable()");
	States::Type newState = States::ParkedState;
	model->getILC()->writeSetModeEnableBuffer();
	model->getILC()->triggerModbus();
	model->getILC()->waitForAllSubnets(5000);
	model->getILC()->readAll();
	model->getILC()->verifyResponses();
	model->getDigitalInputOutput()->turnAirOn();
	model->getPowerController()->setAllAuxPowerNetworks(true);
	return model->getSafetyController()->checkSafety(newState);
}

States::Type DisabledState::standby(StandbyCommand* command, Model* model) {
	Log.Info("DisabledState: standby()");
	States::Type newState = States::StandbyState;
	Log.Info("SetModeDisable");
	model->getILC()->writeSetModeDisableBuffer();
	model->getILC()->triggerModbus();
	model->getILC()->waitForAllSubnets(5000);
	model->getILC()->readAll();
	model->getPublisher()->tryLogForceActuatorState();
	Log.Info("SetModeStandby");
	model->getILC()->writeSetModeStandbyBuffer();
	model->getILC()->triggerModbus();
	model->getILC()->waitForAllSubnets(5000);
	model->getILC()->readAll();
	model->getILC()->verifyResponses();
	model->getPublisher()->tryLogForceActuatorState();
	model->getPowerController()->setBothPowerNetworks(false);
	return model->getSafetyController()->checkSafety(newState);
}

States::Type DisabledState::programILC(ProgramILCCommand* command, Model* model) {
	Log.Info("DisabledState: programILC()");
	model->getILC()->programILC(command->getData()->ActuatorId, command->getData()->FilePath);
	return model->getSafetyController()->checkSafety(States::NoStateTransition);
}

States::Type DisabledState::modbusTransmit(ModbusTransmitCommand* command, Model* model) {
	Log.Info("DisabledState: modbusTransmit()");
	model->getILC()->modbusTransmit(command->getData()->ActuatorId, command->getData()->FunctionCode, command->getData()->DataLength, command->getData()->Data);
	return model->getSafetyController()->checkSafety(States::NoStateTransition);
}

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */
