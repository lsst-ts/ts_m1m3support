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

#ifndef FREEZESENSORBUSLIST_H_
#define FREEZESENSORBUSLIST_H_

#include <BusList.h>
#include <SAL_MTM1M3C.h>

namespace LSST {
namespace M1M3 {
namespace SS {

class FreezeSensorBusList : public BusList {
public:
    FreezeSensorBusList(ILCSubnetData* subnetData, ILCMessageFactory* ilcMessageFactory);

    void update();

private:
    MTM1M3_outerLoopDataC* _outerLoopData;

    int32_t _freezeSensorCommandIndex[5];
    int32_t _faStatusCommandIndex[5];
    int32_t _roundRobinFAReportServerStatusIndex[5];
    int32_t _hmLVDTCommandIndex[5];
    int32_t _lvdtSampleClock;
};

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */

#endif /* FREEZESENSORBUSLIST_H_ */
