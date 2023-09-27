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
#ifndef LSST_RAISINGLOWERINGINFO_H
#define LSST_RAISINGLOWERINGINFO_H

#include <SAL_MTM1M3.h>

#include <cRIO/Singleton.h>

namespace LSST {
namespace M1M3 {
namespace SS {

/**
 * Manipulates RaisingLoweringInfo event fields.
 */
class RaisingLoweringInfo : public MTM1M3_logevent_raisingLoweringInfoC,
                            public cRIO::Singleton<RaisingLoweringInfo> {
public:
    RaisingLoweringInfo(token);

    /**
     * Sends updates to RaisingLoweringInfo structure.
     *
     * @param force if true, send updates even when _updated flag is not set
     */
    void sendUpdates(bool force = false);

    /**
     * Returns ratio of mirror mass supported.
     *
     * @return Mirror supported ration (in 0-1 range, 1 mirror weight fully
     * supported by force actuators)
     */
    float supportRatio() { return weightSupportedPercent / 100.0; }

    void setTimeTimeout(double new_timeTimeout);

    /**
     * Increases mirrror support percentage by RaiseIncrementPercentage setting
     * value.
     */
    void incSupportPercentage();

    /**
     * Decrements mirror support percentage by LowerDecrementPercentage setting
     * value.
     */
    void decSupportPercentage();

    /**
     * Sets support percentage to 0%.
     */
    void zeroSupportPercentage();

    /**
     * Sets support percentage to 100%.
     */
    void fillSupportPercentage();

    /**
     * Is mirror support percentage equal or more than 100%?
     *
     * @return true if the mirror is fully supported by target force actuator
     * values.
     */
    bool supportPercentageFilled();

    /**
     * Is mirror support percentage less or equal to 0?
     *
     * @return true if mirror the mirror is not supported by target force
     * actuator values.
     */
    bool supportPercentageZeroed();

    /**
     * Sets waitHardpoint field, triggers updates if needed.
     */
    void setHPWait(size_t hpIndex, bool newWait);

    void setFAXWait(size_t faXIndex, bool newWait);
    void setFAYWait(size_t faYIndex, bool newWait);
    void setFAZWait(size_t faZIndex, bool newWait);

    /**
     * Sets waitAirPressure field. Sends update if the value changed.
     *
     * @param newWait new waitAirPressure value.
     */
    void setWaitAirPressure(bool newWait);

private:
    bool _updated;
};

}  // namespace SS
}  // namespace M1M3
}  // namespace LSST

#endif  // !LSST_RAISINGLOWERINGINFO_H
