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

#include <sal_MTM1M3.h>
#include <spdlog/spdlog.h>

#include <BoosterValveStatus.h>
#include <Model.h>
#include <Publisher.h>
#include <SlewController.h>

using namespace MTM1M3;

using namespace LSST::M1M3::SS;

SlewController::SlewController() { SPDLOG_DEBUG("SlewController: SlewController()"); }

void SlewController::enterSlew() {
    BoosterValveStatus::instance().enterSlew();
    Model::get().getForceController()->applyVelocityForces();
    Model::get().getForceController()->applyAccelerationForces();
}

void SlewController::exitSlew() {
    BoosterValveStatus::instance().exitSlew();
    Model::get().getForceController()->zeroAccelerationForces();
    Model::get().getForceController()->zeroVelocityForces();
}

void SlewController::reset() { exitSlew(); }
