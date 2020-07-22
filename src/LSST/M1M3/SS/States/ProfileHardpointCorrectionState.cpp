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

#include <ProfileHardpointCorrectionState.h>
#include <ForceController.h>
#include <ProfileController.h>
#include <MirrorForceProfile.h>
#include <SafetyController.h>
#include <spdlog/spdlog.h>

namespace LSST {
namespace M1M3 {
namespace SS {

ProfileHardpointCorrectionState::ProfileHardpointCorrectionState(M1M3SSPublisher* publisher)
        : EnabledState(publisher, "ProfileHardpointCorrectionState") {}

States::Type ProfileHardpointCorrectionState::update(UpdateCommand* command, Model* model) {
    spdlog::trace("ProfileHardpointCorrectionState: update()");
    MirrorForceProfileRecord force = model->getProfileController()->getMirrorForceProfileData();
    model->getForceController()->applyOffsetForcesByMirrorForces(force.XForce, force.YForce, force.ZForce,
                                                                 force.XMoment, force.YMoment, force.ZMoment);
    sendTelemetry(model);
    if (model->getProfileController()->incMirrorForceProfile()) {
        return model->getSafetyController()->checkSafety(States::ActiveEngineeringState);
    }
    return model->getSafetyController()->checkSafety(States::NoStateTransition);
}

States::Type ProfileHardpointCorrectionState::abortProfile(AbortProfileCommand* command, Model* model) {
    spdlog::info("ProfileHardpointCorrectionState: abortProfile()");
    model->getForceController()->zeroOffsetForces();
    return model->getSafetyController()->checkSafety(States::ActiveEngineeringState);
}

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */
