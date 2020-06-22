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

#include <ProfileController.h>
#include <spdlog/spdlog.h>

namespace LSST {
namespace M1M3 {
namespace SS {

ProfileController::ProfileController() { spdlog::debug("ProfileController: ProfileController()"); }

void ProfileController::setupMirrorForceProfile(float* xForce, float* yForce, float* zForce, float* xMoment,
                                                float* yMoment, float* zMoment) {
    spdlog::info("ProfileController: setupMirrorForceProfile()");
    this->mirrorForceProfile.Index = 0;
    for (int i = 0; i < 1000; ++i) {
        this->mirrorForceProfile.Records[i].XForce = xForce[i];
        this->mirrorForceProfile.Records[i].YForce = yForce[i];
        this->mirrorForceProfile.Records[i].ZForce = zForce[i];
        this->mirrorForceProfile.Records[i].XMoment = xMoment[i];
        this->mirrorForceProfile.Records[i].YMoment = yMoment[i];
        this->mirrorForceProfile.Records[i].ZMoment = zMoment[i];
    }
}

bool ProfileController::incMirrorForceProfile() {
    spdlog::trace("ProfileController: incMirrorForceProfile()");
    ++this->mirrorForceProfile.Index;
    return this->mirrorForceProfile.Index >= 1000;
}

MirrorForceProfileRecord ProfileController::getMirrorForceProfileData() {
    return this->mirrorForceProfile.Records[this->mirrorForceProfile.Index];
}

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */
