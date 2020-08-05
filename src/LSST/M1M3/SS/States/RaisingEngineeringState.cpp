/*
 * This file is part of LSST M1M3 support system package.
 *
 * Developed for the LSST Data Management System.
 * This product includes software developed by the LSST Project
 * (https://www.lsst.org).
 * See the COPYRIGHT file at the top-level directory of this distribution
 * for details of code ownership.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#include <RaisingEngineeringState.h>
#include <Model.h>
#include <ModelPublisher.h>
#include <SafetyController.h>
#include <AutomaticOperationsController.h>
#include <spdlog/spdlog.h>

namespace LSST {
namespace M1M3 {
namespace SS {

RaisingEngineeringState::RaisingEngineeringState() : EngineeringState("RaisingEngineeringState") {}

States::Type RaisingEngineeringState::update(UpdateCommand* command, Model* model) {
    ModelPublisher publishModel(model);
    spdlog::trace("RaisingEngineeringState: update()");
    model->getAutomaticOperationsController()->tryIncrementingSupportPercentage();
    runLoop(model);
    return model->getSafetyController()->checkSafety(raiseCompleted(model) ? States::ActiveEngineeringState
                                                                           : States::NoStateTransition);
}

States::Type RaisingEngineeringState::abortRaiseM1M3(AbortRaiseM1M3Command* command, Model* model) {
    spdlog::info("RaisingEngineeringState: abortRaiseM1M3()");
    model->getAutomaticOperationsController()->abortRaiseM1M3();
    return model->getSafetyController()->checkSafety(States::LoweringEngineeringState);
}

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */
