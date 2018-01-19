/*
 * State.cpp
 *
 *  Created on: Sep 26, 2017
 *      Author: ccontaxis
 */

#include <State.h>
#include <DataTypes.h>

namespace LSST {
namespace M1M3 {
namespace SS {

State::~State() { }

States::Type State::boot(BootCommand* command, Model* model) { return States::NoStateTransition; }
States::Type State::start(StartCommand* command, Model* model) { return States::NoStateTransition; }
States::Type State::enable(EnableCommand* command, Model* model) { return States::NoStateTransition; }
States::Type State::disable(DisableCommand* command, Model* model) { return States::NoStateTransition; }
States::Type State::standby(StandbyCommand* command, Model* model) { return States::NoStateTransition; }
States::Type State::shutdown(ShutdownCommand* command, Model* model) { return States::NoStateTransition; }
States::Type State::update(UpdateCommand* command, Model* model) { return States::NoStateTransition; }
States::Type State::turnAirOn(TurnAirOnCommand* command, Model* model) { return States::NoStateTransition; }
States::Type State::turnAirOff(TurnAirOffCommand* command, Model* model) { return States::NoStateTransition; }
States::Type State::applyOffsetForces(ApplyOffsetForcesCommand* command, Model* model) { return States::NoStateTransition; }
States::Type State::clearOffsetForces(ClearOffsetForcesCommand* command, Model* model) { return States::NoStateTransition; }
States::Type State::raiseM1M3(RaiseM1M3Command* command, Model* model) { return States::NoStateTransition; }
States::Type State::lowerM1M3(LowerM1M3Command* command, Model* model) { return States::NoStateTransition; }
States::Type State::applyAberrationByBendingModes(ApplyAberrationByBendingModesCommand* command, Model* model) { return States::NoStateTransition; }
States::Type State::applyAberrationByForces(ApplyAberrationByForcesCommand* command, Model* model) { return States::NoStateTransition; }
States::Type State::clearAberration(ClearAberrationCommand* command, Model* model) { return States::NoStateTransition; }
States::Type State::applyAOSCorrectionByBendingModes(ApplyAOSCorrectionByBendingModesCommand* command, Model* model) { return States::NoStateTransition; }
States::Type State::applyAOSCorrectionByForces(ApplyAOSCorrectionByForcesCommand* command, Model* model) { return States::NoStateTransition; }
States::Type State::clearAOSCorrection(ClearAOSCorrectionCommand* command, Model* model) { return States::NoStateTransition; }
States::Type State::enterEngineering(EnterEngineeringCommand* command, Model* model) { return States::NoStateTransition; }
States::Type State::exitEngineering(ExitEngineeringCommand* command, Model* model) { return States::NoStateTransition; }
States::Type State::testAir(TestAirCommand* command, Model* model) { return States::NoStateTransition; }
States::Type State::testHardpoint(TestHardpointCommand* command, Model* model) { return States::NoStateTransition; }
States::Type State::testForceActuator(TestForceActuatorCommand* command, Model* model) { return States::NoStateTransition; }
States::Type State::moveHardpointActuators(MoveHardpointActuatorsCommand* command, Model* model) { return States::NoStateTransition; }
States::Type State::enableHardpointChase(EnableHardpointChaseCommand* command, Model* model) { return States::NoStateTransition; }
States::Type State::disableHardpointChase(DisableHardpointChaseCommand* command, Model* model) { return States::NoStateTransition; }
States::Type State::abortRaiseM1M3(AbortRaiseM1M3Command* command, Model* model) { return States::NoStateTransition; }
States::Type State::translateM1M3(TranslateM1M3Command* command, Model* model) { return States::NoStateTransition; }
States::Type State::stopHardpointMotion(StopHardpointMotionCommand* command, Model* model) { return States::NoStateTransition; }
States::Type State::storeTMAAzimuthSample(TMAAzimuthSampleCommand* command, Model* model) { return States::NoStateTransition; }
States::Type State::storeTMAElevationSample(TMAElevationSampleCommand* command, Model* model) { return States::NoStateTransition; }
States::Type State::positionM1M3(PositionM1M3Command* command, Model* model) { return States::NoStateTransition; }
States::Type State::turnLightsOn(TurnLightsOnCommand* command, Model* model) { return States::NoStateTransition; }
States::Type State::turnLightsOff(TurnLightsOffCommand* command, Model* model) { return States::NoStateTransition; }
States::Type State::turnPowerOn(TurnPowerOnCommand* command, Model* model) { return States::NoStateTransition; }
States::Type State::turnPowerOff(TurnPowerOffCommand* command, Model* model) { return States::NoStateTransition; }

void State::startTimer() {
	clock_gettime(CLOCK_REALTIME, &this->startTime);
}

void State::stopTimer() {
	clock_gettime(CLOCK_REALTIME, &this->stopTime);
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

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */
