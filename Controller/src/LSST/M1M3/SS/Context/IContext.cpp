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

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */
