/*
 * This file is part of LSST M1M3 support system package.
 *
 * Developed for the Vera C. Rubin Telescope and Site System.
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

#include <ForceController.h>
#include <MirrorForceProfile.h>
#include <ProfileController.h>
#include <ProfileHardpointCorrectionState.h>
#include <SafetyController.h>
#include <spdlog/spdlog.h>

namespace LSST {
namespace M1M3 {
namespace SS {

ProfileHardpointCorrectionState::ProfileHardpointCorrectionState()
        : EnabledState("ProfileHardpointCorrectionState") {}

States::Type ProfileHardpointCorrectionState::update(UpdateCommand* command) {
    SPDLOG_TRACE("ProfileHardpointCorrectionState: update()");
    MirrorForceProfileRecord force = Model::instance().getProfileController()->getMirrorForceProfileData();
    Model::instance().getForceController()->applyOffsetForcesByMirrorForces(
            force.XForce, force.YForce, force.ZForce, force.XMoment, force.YMoment, force.ZMoment);
    sendTelemetry();
    if (Model::instance().getProfileController()->incMirrorForceProfile()) {
        return Model::instance().getSafetyController()->checkSafety(States::ActiveEngineeringState);
    }
    return Model::instance().getSafetyController()->checkSafety(States::NoStateTransition);
}

States::Type ProfileHardpointCorrectionState::abortProfile(AbortProfileCommand* command) {
    SPDLOG_INFO("ProfileHardpointCorrectionState: abortProfile()");
    Model::instance().getForceController()->zeroOffsetForces();
    return Model::instance().getSafetyController()->checkSafety(States::ActiveEngineeringState);
}

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */
