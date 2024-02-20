/*
 * This file is part of LSST M1M3 support system package.
 *
 * Developed for the Telescope & Site Software Systems.
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
#ifndef WITH_SAL_KAFKA
#include <sal_MTM1M3.h>
#endif

#include <spdlog/spdlog.h>

#include <BoosterValveStatus.h>
#include <ForceControllerState.h>
#include <Model.h>
#include <M1M3SSPublisher.h>
#include <SettingReader.h>
#include <SlewController.h>
#include <SlewControllerSettings.h>

using namespace MTM1M3;

using namespace LSST::M1M3::SS;

SlewController::SlewController() { SPDLOG_DEBUG("SlewController: SlewController()"); }

void SlewController::enterSlew() {
    ForceControllerState::instance().set_slewFlag(true);
    auto &slew_settings = SlewControllerSettings::instance();
    if (slew_settings.triggerBoosterValves) {
        BoosterValveStatus::instance().enterSlew();
    }
    if (slew_settings.useVelocityForces) {
        Model::instance().getForceController()->applyVelocityForces();
    }
    if (slew_settings.useAccelerationForces) {
        Model::instance().getForceController()->applyAccelerationForces();
    }
    if (slew_settings.useBalanceForces) {
        auto pidSettings = SettingReader::instance().getPIDSettings(true);
        for (int i = 0; i < 6; i++) {
            Model::instance().getForceController()->updatePID(i, pidSettings.getParameters(i));
        }
        _balanceForcesEnabled = Model::instance().getForceController()->applyBalanceForces();
    } else {
        _balanceForcesEnabled = Model::instance().getForceController()->zeroBalanceForces();
    }
}

void SlewController::exitSlew() {
    auto &slew_settings = SlewControllerSettings::instance();
    if (slew_settings.triggerBoosterValves) {
        BoosterValveStatus::instance().exitSlew();
    }
    if (slew_settings.useAccelerationForces) {
        Model::instance().getForceController()->zeroAccelerationForces();
    }
    if (slew_settings.useVelocityForces) {
        Model::instance().getForceController()->zeroVelocityForces();
    }
    if (slew_settings.useBalanceForces) {
        auto pidSettings = SettingReader::instance().getPIDSettings(false);
        for (int i = 0; i < 6; i++) {
            Model::instance().getForceController()->updatePID(i, pidSettings.getParameters(i));
        }
    }
    if (_balanceForcesEnabled) {
        Model::instance().getForceController()->applyBalanceForces();
    } else {
        Model::instance().getForceController()->zeroBalanceForces();
    }
    ForceControllerState::instance().set_slewFlag(false);
}

void SlewController::reset() { exitSlew(); }
