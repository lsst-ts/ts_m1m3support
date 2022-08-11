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

#ifndef ACTIVEBUSLIST_H_
#define ACTIVEBUSLIST_H_

#include <BusList.h>
#include <SAL_MTM1M3C.h>

namespace LSST {
namespace M1M3 {
namespace SS {

class ActiveBusList : public BusList {
public:
    ActiveBusList(ILCSubnetData* subnetData, ILCMessageFactory* ilcMessageFactory);

    void buildBuffer() override;
    void update() override;

private:
    MTM1M3_outerLoopDataC* _outerLoopData;
    MTM1M3_forceActuatorDataC* _forceActuatorData;
    MTM1M3_appliedCylinderForcesC* _appliedCylinderForces;
    MTM1M3_hardpointActuatorDataC* _hardpointActuatorData;
    MTM1M3_logevent_forceActuatorInfoC* _forceInfo;

    int32_t _setForceCommandIndex[5];
    int32_t _hpFreezeCommandIndex[5];
    int32_t _faStatusCommandIndex[5];
    int32_t _roundRobinFAReportServerStatusIndex[5];
    int32_t _hmLVDTCommandIndex[5];
    int32_t _lvdtSampleClock;
};

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */

#endif /* ACTIVEBUSLIST_H_ */
