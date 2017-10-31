/*
 * ISubscriber.cpp
 *
 *  Created on: Sep 27, 2017
 *      Author: ccontaxis
 */

#include <ISubscriber.h>

namespace LSST {
namespace M1M3 {
namespace SS {

ISubscriber::~ISubscriber() { }

ICommand* ISubscriber::tryAcceptCommandStart() { return 0; }
ICommand* ISubscriber::tryAcceptCommandEnable() { return 0; }
ICommand* ISubscriber::tryAcceptCommandDisable() { return 0; }
ICommand* ISubscriber::tryAcceptCommandStandby() { return 0; }
ICommand* ISubscriber::tryAcceptCommandShutdown() { return 0; }
ICommand* ISubscriber::tryAcceptCommandTurnAirOn() { return 0; }
ICommand* ISubscriber::tryAcceptCommandTurnAirOff() { return 0; }
ICommand* ISubscriber::tryAcceptCommandApplyOffsetForces() { return 0; }
ICommand* ISubscriber::tryAcceptCommandClearOffsetForces() { return 0; }
ICommand* ISubscriber::tryAcceptCommandRaiseM1M3() { return 0; }
ICommand* ISubscriber::tryAcceptCommandLowerM1M3() { return 0; }
ICommand* ISubscriber::tryAcceptCommandApplyAberrationByBendingModes() { return 0; }
ICommand* ISubscriber::tryAcceptCommandApplyAberrationByForces() { return 0; }
ICommand* ISubscriber::tryAcceptCommandClearAberration() { return 0; }
ICommand* ISubscriber::tryAcceptCommandApplyAOSCorrectionByBendingModes() { return 0; }
ICommand* ISubscriber::tryAcceptCommandApplyAOSCorrectionByForces() { return 0; }
ICommand* ISubscriber::tryAcceptCommandClearAOSCorrection() { return 0; }
ICommand* ISubscriber::tryAcceptCommandEnterEngineering() { return 0; }
ICommand* ISubscriber::tryAcceptCommandExitEngineering() { return 0; }
ICommand* ISubscriber::tryAcceptCommandTestAir() { return 0; }
ICommand* ISubscriber::tryAcceptCommandTestHardpoint() { return 0; }
ICommand* ISubscriber::tryAcceptCommandTestForceActuator() { return 0; }
ICommand* ISubscriber::tryAcceptCommandMoveHardpointActuators() { return 0; }
ICommand* ISubscriber::tryAcceptCommandEnableHardpointChase() { return 0; }
ICommand* ISubscriber::tryAcceptCommandDisableHardpointChase() { return 0; }
ICommand* ISubscriber::tryAcceptCommandAbortRaiseM1M3() { return 0; }
ICommand* ISubscriber::tryAcceptCommandTranslateM1M3() { return 0; }
ICommand* ISubscriber::tryAcceptCommandStopHardpointMotion() { return 0; }

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */
