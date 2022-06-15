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

#ifndef TMA_H_
#define TMA_H_

#include <SAL_MTMountC.h>

#include <cRIO/Singleton.h>

#include <cmath>

namespace LSST {
namespace M1M3 {
namespace SS {

/**
 * Represents Telescope Mount Assembly (TMA). Provides functions to retrieve
 * mirror elevation, either from inclinometer readout, or from telescope data.
 */
class TMA : public cRIO::Singleton<TMA> {
public:
    TMA(token);

    /**
     * Checks that TMA azimuth and elevation timestamps are current.
     *
     * @param checkAzimuth true if azimuth timestamp shall be checked
     * @param checkElevation true if elevation timestamp shall be checked
     */
    void checkTimestamps(bool checkAzimuth, bool checkElevation);

    /**
     * Updates azimuth data to match current TMA data. Should be called on reception of new azimuth data.
     *
     * @param data MTMount_azimuth data
     */
    void updateTMAAzimuth(MTMount_azimuthC* data);

    /**
     * Updates elevation data to match current TMA data. Should be called on reception of new elevation data.
     *
     * @param data MTMount_elevation data
     */
    void updateTMAElevation(MTMount_elevationC* data);

    /**
     * Returns mirror elevation. Uses either telescope provided data, or internal inclinometer readout.
     *
     * @return telescope elevation in degrees. 0 for horizon, 90 for zenith.
     */
    double getElevation();

    /**
     * Returns elevation sin.
     *
     * @return elvation sin
     */
    double getElevationSin() { return sin(getElevation() * D2RAD); }

    /**
     * Returns elevation cos.
     *
     * @return elevation cos
     */
    double getElevationCos() { return cos(getElevation() * D2RAD); }

private:
    double _azimuth_Timestamp;
    double _azimuth_Actual;

    double _elevation_Timestamp;
    double _elevation_Actual;
};

}  // namespace SS
}  // namespace M1M3
}  // namespace LSST

#endif  //! TMA_TMA_H_
