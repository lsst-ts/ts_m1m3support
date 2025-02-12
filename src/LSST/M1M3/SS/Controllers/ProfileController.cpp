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

#include <ProfileController.h>
#include <spdlog/spdlog.h>

namespace LSST {
namespace M1M3 {
namespace SS {

ProfileController::ProfileController() { SPDLOG_DEBUG("ProfileController: ProfileController()"); }

#ifdef WITH_SAL_KAFKA
void ProfileController::setupMirrorForceProfile(const std::vector<float> &xForce,
                                                const std::vector<float> &yForce,
                                                const std::vector<float> &zForce,
                                                const std::vector<float> &xMoment,
                                                const std::vector<float> &yMoment,
                                                const std::vector<float> &zMoment) {
#else
void ProfileController::setupMirrorForceProfile(float *xForce, float *yForce, float *zForce, float *xMoment,
                                                float *yMoment, float *zMoment) {
#endif
    SPDLOG_INFO("ProfileController: setupMirrorForceProfile()");
    _mirrorForceProfile.Index = 0;
    for (int i = 0; i < 1000; ++i) {
        _mirrorForceProfile.Records[i].XForce = xForce[i];
        _mirrorForceProfile.Records[i].YForce = yForce[i];
        _mirrorForceProfile.Records[i].ZForce = zForce[i];
        _mirrorForceProfile.Records[i].XMoment = xMoment[i];
        _mirrorForceProfile.Records[i].YMoment = yMoment[i];
        _mirrorForceProfile.Records[i].ZMoment = zMoment[i];
    }
}

bool ProfileController::incMirrorForceProfile() {
    SPDLOG_TRACE("ProfileController: incMirrorForceProfile()");
    ++_mirrorForceProfile.Index;
    return _mirrorForceProfile.Index >= 1000;
}

MirrorForceProfileRecord ProfileController::getMirrorForceProfileData() {
    return _mirrorForceProfile.Records[_mirrorForceProfile.Index];
}

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */
