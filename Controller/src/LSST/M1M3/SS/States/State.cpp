/*
 * State.cpp
 *
 *  Created on: Sep 26, 2017
 *      Author: ccontaxis
 */

#include <State.h>

namespace LSST {
namespace M1M3 {
namespace SS {

States::Type State::boot(BootCommand* command, IModel* model) { return States::Ignore; }
States::Type State::start(StartCommand* command, IModel* model) { return States::Ignore; }
States::Type State::enable(EnableCommand* command, IModel* model) { return States::Ignore; }
States::Type State::disable(DisableCommand* command, IModel* model) { return States::Ignore; }
States::Type State::standby(StandbyCommand* command, IModel* model) { return States::Ignore; }
States::Type State::shutdown(ShutdownCommand* command, IModel* model) { return States::Ignore; }
States::Type State::update(UpdateCommand* command, IModel* model) { return States::Ignore; }
States::Type State::turnAirOn(TurnAirOnCommand* command, IModel* model) { return States::Ignore; }
States::Type State::turnAirOff(TurnAirOffCommand* command, IModel* model) { return States::Ignore; }
States::Type State::applyOffsetForces(ApplyOffsetForcesCommand* command, IModel* model) { return States::Ignore; }
States::Type State::clearOffsetForces(ClearOffsetForcesCommand* command, IModel* model) { return States::Ignore; }
States::Type State::raiseM1M3(RaiseM1M3Command* command, IModel* model) { return States::Ignore; }
States::Type State::lowerM1M3(LowerM1M3Command* command, IModel* model) { return States::Ignore; }

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */
