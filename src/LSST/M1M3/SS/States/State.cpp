#include <State.h>
#include <DataTypes.h>
#include <M1M3SSPublisher.h>
#include <spdlog/spdlog.h>

namespace LSST {
namespace M1M3 {
namespace SS {

State::State(M1M3SSPublisher* publisher, std::string name) {
    this->publisher = publisher;
    this->name = name;
}

State::~State() {}

States::Type State::boot(BootCommand* command, Model* model) { return States::NoStateTransition; }
States::Type State::start(StartCommand* command, Model* model) {
    return this->rejectCommandInvalidState("Start");
}
States::Type State::enable(EnableCommand* command, Model* model) {
    return this->rejectCommandInvalidState("Enable");
}
States::Type State::disable(DisableCommand* command, Model* model) {
    return this->rejectCommandInvalidState("Disable");
}
States::Type State::standby(StandbyCommand* command, Model* model) {
    return this->rejectCommandInvalidState("Standby");
}
States::Type State::shutdown(ShutdownCommand* command, Model* model) {
    return this->rejectCommandInvalidState("Shutdown");
}
States::Type State::update(UpdateCommand* command, Model* model) { return States::NoStateTransition; }
States::Type State::turnAirOn(TurnAirOnCommand* command, Model* model) {
    return this->rejectCommandInvalidState("TurnAirOn");
}
States::Type State::turnAirOff(TurnAirOffCommand* command, Model* model) {
    return this->rejectCommandInvalidState("TurnAirOff");
}
States::Type State::applyOffsetForces(ApplyOffsetForcesCommand* command, Model* model) {
    return this->rejectCommandInvalidState("ApplyOffsetForces");
}
States::Type State::clearOffsetForces(ClearOffsetForcesCommand* command, Model* model) {
    return this->rejectCommandInvalidState("ClearOffsetForces");
}
States::Type State::raiseM1M3(RaiseM1M3Command* command, Model* model) {
    return this->rejectCommandInvalidState("RaiseM1M3");
}
States::Type State::lowerM1M3(LowerM1M3Command* command, Model* model) {
    return this->rejectCommandInvalidState("LowerM1M3");
}
States::Type State::applyAberrationForcesByBendingModes(ApplyAberrationForcesByBendingModesCommand* command,
                                                        Model* model) {
    return this->rejectCommandInvalidState("ApplyAberrationForcesByBendingModes");
}
States::Type State::applyAberrationForces(ApplyAberrationForcesCommand* command, Model* model) {
    return this->rejectCommandInvalidState("ApplyAberrationForces");
}
States::Type State::clearAberrationForces(ClearAberrationForcesCommand* command, Model* model) {
    return this->rejectCommandInvalidState("ClearAberrationForces");
}
States::Type State::applyActiveOpticForcesByBendingModes(ApplyActiveOpticForcesByBendingModesCommand* command,
                                                         Model* model) {
    return this->rejectCommandInvalidState("ApplyActiveOpticForcesByBendingModes");
}
States::Type State::applyActiveOpticForces(ApplyActiveOpticForcesCommand* command, Model* model) {
    return this->rejectCommandInvalidState("ApplyActiveOpticForces");
}
States::Type State::clearActiveOpticForces(ClearActiveOpticForcesCommand* command, Model* model) {
    return this->rejectCommandInvalidState("ClearActiveOpticForces");
}
States::Type State::enterEngineering(EnterEngineeringCommand* command, Model* model) {
    return this->rejectCommandInvalidState("EnterEngineering");
}
States::Type State::exitEngineering(ExitEngineeringCommand* command, Model* model) {
    return this->rejectCommandInvalidState("ExitEngineering");
}
States::Type State::testAir(TestAirCommand* command, Model* model) {
    return this->rejectCommandInvalidState("TestAir");
}
States::Type State::testHardpoint(TestHardpointCommand* command, Model* model) {
    return this->rejectCommandInvalidState("TestHardpoint");
}
States::Type State::testForceActuator(TestForceActuatorCommand* command, Model* model) {
    return this->rejectCommandInvalidState("TestForceActuator");
}
States::Type State::moveHardpointActuators(MoveHardpointActuatorsCommand* command, Model* model) {
    return this->rejectCommandInvalidState("MoveHardpointActuators");
}
States::Type State::enableHardpointChase(EnableHardpointChaseCommand* command, Model* model) {
    return this->rejectCommandInvalidState("EnableHardpointChase");
}
States::Type State::disableHardpointChase(DisableHardpointChaseCommand* command, Model* model) {
    return this->rejectCommandInvalidState("DisableHardpointChase");
}
States::Type State::abortRaiseM1M3(AbortRaiseM1M3Command* command, Model* model) {
    return this->rejectCommandInvalidState("AbortRaiseM1M3");
}
States::Type State::translateM1M3(TranslateM1M3Command* command, Model* model) {
    return this->rejectCommandInvalidState("TranslateM1M3");
}
States::Type State::stopHardpointMotion(StopHardpointMotionCommand* command, Model* model) {
    return this->rejectCommandInvalidState("StopHardpointMotion");
}
States::Type State::storeTMAAzimuthSample(TMAAzimuthSampleCommand* command, Model* model) {
    return States::NoStateTransition;
}
States::Type State::storeTMAElevationSample(TMAElevationSampleCommand* command, Model* model) {
    return States::NoStateTransition;
}
States::Type State::positionM1M3(PositionM1M3Command* command, Model* model) {
    return this->rejectCommandInvalidState("PositionM1M3");
}
States::Type State::turnLightsOn(TurnLightsOnCommand* command, Model* model) {
    return this->rejectCommandInvalidState("TurnLightsOn");
}
States::Type State::turnLightsOff(TurnLightsOffCommand* command, Model* model) {
    return this->rejectCommandInvalidState("TurnLightsOff");
}
States::Type State::turnPowerOn(TurnPowerOnCommand* command, Model* model) {
    return this->rejectCommandInvalidState("TurnPowerOn");
}
States::Type State::turnPowerOff(TurnPowerOffCommand* command, Model* model) {
    return this->rejectCommandInvalidState("TurnPowerOff");
}
States::Type State::enableHardpointCorrections(EnableHardpointCorrectionsCommand* command, Model* model) {
    return this->rejectCommandInvalidState("EnableHardpointCorrections");
}
States::Type State::disableHardpointCorrections(DisableHardpointCorrectionsCommand* command, Model* model) {
    return this->rejectCommandInvalidState("DisableHardpointCorrections");
}
States::Type State::runMirrorForceProfile(RunMirrorForceProfileCommand* command, Model* model) {
    return this->rejectCommandInvalidState("RunMirrorForceProfile");
}
States::Type State::abortProfile(AbortProfileCommand* command, Model* model) {
    return this->rejectCommandInvalidState("AbortProfile");
}
States::Type State::applyOffsetForcesByMirrorForce(ApplyOffsetForcesByMirrorForceCommand* command,
                                                   Model* model) {
    return this->rejectCommandInvalidState("ApplyOffsetForcesByMirrorForce");
}
States::Type State::updatePID(UpdatePIDCommand* command, Model* model) {
    return this->rejectCommandInvalidState("UpdatePID");
}
States::Type State::resetPID(ResetPIDCommand* command, Model* model) {
    return this->rejectCommandInvalidState("ResetPID");
}
States::Type State::programILC(ProgramILCCommand* command, Model* model) {
    return this->rejectCommandInvalidState("ProgramILC");
}
States::Type State::modbusTransmit(ModbusTransmitCommand* command, Model* model) {
    return this->rejectCommandInvalidState("ModbusTransmit");
}

void State::startTimer() { clock_gettime(CLOCK_REALTIME, &this->startTime); }

void State::stopTimer() { clock_gettime(CLOCK_REALTIME, &this->stopTime); }

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

States::Type State::rejectCommandInvalidState(std::string command) {
    spdlog::warn("The command %s is not valid in the {}.", command, this->name);
    this->publisher->logCommandRejectionWarning(
            command, "The command " + command + " is not valid in the " + this->name + ".");
    return States::NoStateTransition;
}

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */
