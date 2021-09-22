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

#include <ForceActuatorPressure.h>
#include <M1M3SSPublisher.h>

namespace LSST {
namespace M1M3 {
namespace SS {

ForceActuatorPressure::ForceActuatorPressure() {
    for (size_t primaryIndex = 0; primaryIndex < FA_PRIMARY_COUNT; primaryIndex++) {
       primaryCylinderPullPressures[primaryIndex] = NAN;
       primaryCylinderPushPressures[primaryIndex] = NAN;
    }

    for (size_t secondaryIndex = 0; secondaryIndex < FA_SECONDARY_COUNT; secondaryIndex++) {
       secondaryCylinderPullPressures[secondaryIndex] = NAN;
       secondaryCylinderPushPressures[secondaryIndex] = NAN;
    }
}

void ForceActuatorPressure::parseReadDCAPressureValuesResponse(ModbusBuffer* buffer, int32_t primaryIndex, int32_t secondaryIndex) {
    primaryCylinderPushPressures[primaryIndex] = buffer->readSGL();
    primaryCylinderPullPressures[primaryIndex] = buffer->readSGL();
    if (secondaryIndex >= 0) {
        secondaryCylinderPullPressures[secondaryIndex] = buffer->readSGL();
        secondaryCylinderPushPressures[secondaryIndex] = buffer->readSGL();

    }
    buffer->skipToNextFrame();
}

}  // namespace SS
}  // namespace M1M3
}  // namespace LSST
