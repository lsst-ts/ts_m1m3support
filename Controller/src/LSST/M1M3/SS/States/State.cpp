/*
 * State.cpp
 *
 *  Created on: Sep 26, 2017
 *      Author: ccontaxis
 */

#include <State.h>
#include <DataTypes.h>
#include <M1M3SSPublisher.h>
#include <Command.h>
#include <Log.h>

namespace LSST {
namespace M1M3 {
namespace SS {

State::State(M1M3SSPublisher* publisher, std::string name) {
    this->publisher = publisher;
    this->name = name;
}

State::~State() {}

States::Type State::boot(BootCommand* command, Model* model) { return States::NoStateTransition; }
States::Type State::start(StartCommand* command, Model* model) { return this->rejectCommandInvalidState((Command*)command); }
States::Type State::enable(EnableCommand* command, Model* model) { return this->rejectCommandInvalidState((Command*)command); }
States::Type State::disable(DisableCommand* command, Model* model) { return this->rejectCommandInvalidState((Command*)command); }
States::Type State::standby(StandbyCommand* command, Model* model) { return this->rejectCommandInvalidState((Command*)command); }
States::Type State::shutdown(ShutdownCommand* command, Model* model) { return this->rejectCommandInvalidState((Command*)command); }
States::Type State::update(UpdateCommand* command, Model* model) { return States::NoStateTransition; }
States::Type State::turnAirOn(TurnAirOnCommand* command, Model* model) { return this->rejectCommandInvalidState((Command*)command); }
States::Type State::turnAirOff(TurnAirOffCommand* command, Model* model) { return this->rejectCommandInvalidState((Command*)command); }
States::Type State::applyOffsetForces(ApplyOffsetForcesCommand* command, Model* model) { return this->rejectCommandInvalidState((Command*)command); }
States::Type State::clearOffsetForces(ClearOffsetForcesCommand* command, Model* model) { return this->rejectCommandInvalidState((Command*)command); }
States::Type State::raiseM1M3(RaiseM1M3Command* command, Model* model) { return this->rejectCommandInvalidState((Command*)command); }
States::Type State::lowerM1M3(LowerM1M3Command* command, Model* model) { return this->rejectCommandInvalidState((Command*)command); }
States::Type State::applyAberrationForcesByBendingModes(ApplyAberrationForcesByBendingModesCommand* command, Model* model) { return this->rejectCommandInvalidState((Command*)command); }
States::Type State::applyAberrationForces(ApplyAberrationForcesCommand* command, Model* model) { return this->rejectCommandInvalidState((Command*)command); }
States::Type State::clearAberrationForces(ClearAberrationForcesCommand* command, Model* model) { return this->rejectCommandInvalidState((Command*)command); }
States::Type State::applyActiveOpticForcesByBendingModes(ApplyActiveOpticForcesByBendingModesCommand* command, Model* model) { return this->rejectCommandInvalidState((Command*)command); }
States::Type State::applyActiveOpticForces(ApplyActiveOpticForcesCommand* command, Model* model) { return this->rejectCommandInvalidState((Command*)command); }
States::Type State::clearActiveOpticForces(ClearActiveOpticForcesCommand* command, Model* model) { return this->rejectCommandInvalidState((Command*)command); }
States::Type State::enterEngineering(EnterEngineeringCommand* command, Model* model) { return this->rejectCommandInvalidState((Command*)command); }
States::Type State::exitEngineering(ExitEngineeringCommand* command, Model* model) { return this->rejectCommandInvalidState((Command*)command); }
States::Type State::testAir(TestAirCommand* command, Model* model) { return this->rejectCommandInvalidState((Command*)command); }
States::Type State::testHardpoint(TestHardpointCommand* command, Model* model) { return this->rejectCommandInvalidState((Command*)command); }
States::Type State::testForceActuator(TestForceActuatorCommand* command, Model* model) { return this->rejectCommandInvalidState((Command*)command); }
States::Type State::moveHardpointActuators(MoveHardpointActuatorsCommand* command, Model* model) { return this->rejectCommandInvalidState((Command*)command); }
States::Type State::enableHardpointChase(EnableHardpointChaseCommand* command, Model* model) { return this->rejectCommandInvalidState((Command*)command); }
States::Type State::disableHardpointChase(DisableHardpointChaseCommand* command, Model* model) { return this->rejectCommandInvalidState((Command*)command); }
States::Type State::abortRaiseM1M3(AbortRaiseM1M3Command* command, Model* model) { return this->rejectCommandInvalidState((Command*)command); }
States::Type State::translateM1M3(TranslateM1M3Command* command, Model* model) { return this->rejectCommandInvalidState((Command*)command); }
States::Type State::stopHardpointMotion(StopHardpointMotionCommand* command, Model* model) { return this->rejectCommandInvalidState((Command*)command); }
States::Type State::storeTMAAzimuthSample(TMAAzimuthSampleCommand* command, Model* model) { return States::NoStateTransition; }
States::Type State::storeTMAElevationSample(TMAElevationSampleCommand* command, Model* model) { return States::NoStateTransition; }
States::Type State::positionM1M3(PositionM1M3Command* command, Model* model) { return this->rejectCommandInvalidState((Command*)command); }
States::Type State::turnLightsOn(TurnLightsOnCommand* command, Model* model) { return this->rejectCommandInvalidState((Command*)command); }
States::Type State::turnLightsOff(TurnLightsOffCommand* command, Model* model) { return this->rejectCommandInvalidState((Command*)command); }
States::Type State::turnPowerOn(TurnPowerOnCommand* command, Model* model) { return this->rejectCommandInvalidState((Command*)command); }
States::Type State::turnPowerOff(TurnPowerOffCommand* command, Model* model) { return this->rejectCommandInvalidState((Command*)command); }
States::Type State::enableHardpointCorrections(EnableHardpointCorrectionsCommand* command, Model* model) { return this->rejectCommandInvalidState((Command*)command); }
States::Type State::disableHardpointCorrections(DisableHardpointCorrectionsCommand* command, Model* model) { return this->rejectCommandInvalidState((Command*)command); }
States::Type State::applyOffsetForcesByMirrorForce(ApplyOffsetForcesByMirrorForceCommand* command, Model* model) { return this->rejectCommandInvalidState((Command*)command); }
States::Type State::updatePID(UpdatePIDCommand* command, Model* model) { return this->rejectCommandInvalidState((Command*)command); }
States::Type State::resetPID(ResetPIDCommand* command, Model* model) { return this->rejectCommandInvalidState((Command*)command); }
States::Type State::programILC(ProgramILCCommand* command, Model* model) { return this->rejectCommandInvalidState((Command*)command); }
States::Type State::modbusTransmit(ModbusTransmitCommand* command, Model* model) { return this->rejectCommandInvalidState((Command*)command); }

void State::startTimer() {
    clock_gettime(CLOCK_REALTIME, &this->startTime);
}

void State::stopTimer() {
    clock_gettime(CLOCK_REALTIME, &this->stopTime);
}

double State::getCurrentTimer() {
    timespec now;
    clock_gettime(CLOCK_REALTIME, &now);
    double deltaNano = now.tv_nsec - this->startTime.tv_nsec;
    double deltaSec = now.tv_sec - this->startTime.tv_sec;
    if (deltaNano < 0) {
        deltaSec -= 1;
        deltaNano += 1000000000;
    }
    return deltaSec + (deltaNano / 1000000000.0);
}

double State::getTimer() {
    double deltaNano = this->stopTime.tv_nsec - this->startTime.tv_nsec;
    double deltaSec = this->stopTime.tv_sec - this->startTime.tv_sec;
    if (deltaNano < 0) {
        deltaSec -= 1;
        deltaNano += 1000000000;
    }
    return deltaSec + (deltaNano / 1000000000.0);
}

States::Type State::rejectCommandInvalidState(Command* command) {
    Log.Warn("The command is not valid in the %s.", this->name.c_str());
    command->ackInvalidState("Not valid in the " + this->name + " state.");
    return States::NoStateTransition;
}

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */
