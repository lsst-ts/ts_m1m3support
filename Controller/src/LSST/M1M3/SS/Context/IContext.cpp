/*
 * IContext.cpp
 *
 *  Created on: Sep 26, 2017
 *      Author: ccontaxis
 */

#include <IContext.h>

namespace LSST {
namespace M1M3 {
namespace SS {

IContext::~IContext() { }

void IContext::boot(BootCommand* command) { }
void IContext::start(StartCommand* command) { }
void IContext::enable(EnableCommand* command) { }
void IContext::disable(DisableCommand* command) { }
void IContext::standby(StandbyCommand* command) { }
void IContext::shutdown(ShutdownCommand* command) { }
void IContext::update(UpdateCommand* command) { }
void IContext::turnAirOn(TurnAirOnCommand* command) { }
void IContext::turnAirOff(TurnAirOffCommand* command) { }
void IContext::applyOffsetForces(ApplyOffsetForcesCommand* command) { }
void IContext::clearOffsetForces(ClearOffsetForcesCommand* command) { }
void IContext::raiseM1M3(RaiseM1M3Command* command) { }
void IContext::lowerM1M3(LowerM1M3Command* command) { }
void IContext::applyAberrationByBendingModes(ApplyAberrationByBendingModesCommand* command) { }
void IContext::applyAberrationByForces(ApplyAberrationByForcesCommand* command) { }
void IContext::clearAberration(ClearAberrationCommand* command) { }
void IContext::applyAOSCorrectionByBendingModes(ApplyAOSCorrectionByBendingModesCommand* command) { }
void IContext::applyAOSCorrectionByForces(ApplyAOSCorrectionByForcesCommand* command) { }
void IContext::clearAOSCorrection(ClearAOSCorrectionCommand* command) { }
void IContext::enterEngineering(EnterEngineeringCommand* command) { }
void IContext::exitEngineering(ExitEngineeringCommand* command) { }
void IContext::testAir(TestAirCommand* command) { }
void IContext::testHardpoint(TestHardpointCommand* command) { }
void IContext::testForceActuator(TestForceActuatorCommand* command) { }
void IContext::moveHardpointActuators(MoveHardpointActuatorsCommand* command) { }
void IContext::enableHardpointChase(EnableHardpointChaseCommand* command) { }
void IContext::disableHardpointChase(DisableHardpointChaseCommand* command) { }
void IContext::abortRaiseM1M3(AbortRaiseM1M3Command* command) { }
void IContext::translateM1M3(TranslateM1M3Command* command) { }
void IContext::stopHardpointMotion(StopHardpointMotionCommand* command) { }
void IContext::storeTMAAzimuthSample(TMAAzimuthSampleCommand* command) { }
void IContext::storeTMAElevationSample(TMAElevationSampleCommand* command) { }
void IContext::positionM1M3(PositionM1M3Command* command) { }

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */
