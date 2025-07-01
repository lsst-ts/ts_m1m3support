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

#ifndef FORCECONVERTER_H_
#define FORCECONVERTER_H_

#include <DistributedForces.h>
#include <ForcesAndMoments.h>
#include <cRIO/DataTypes.h>
#include <cmath>

namespace LSST {
namespace M1M3 {
namespace SS {

class ForceConverter {
public:
    static void daaPositiveXToMirror(float primaryCylinder, float secondaryCylinder, float *xForce,
                                     float *yForce, float *zForce) {
        *xForce = secondaryCylinder * _reciprocalSqrt2;
        *yForce = 0;
        *zForce = secondaryCylinder * _reciprocalSqrt2 + primaryCylinder;
    }

    static void daaNegativeXToMirror(float primaryCylinder, float secondaryCylinder, float *xForce,
                                     float *yForce, float *zForce) {
        *xForce = -secondaryCylinder * _reciprocalSqrt2;
        *yForce = 0;
        *zForce = secondaryCylinder * _reciprocalSqrt2 + primaryCylinder;
    }

    static void daaPositiveYToMirror(float primaryCylinder, float secondaryCylinder, float *xForce,
                                     float *yForce, float *zForce) {
        *xForce = 0;
        *yForce = secondaryCylinder * _reciprocalSqrt2;
        *zForce = secondaryCylinder * _reciprocalSqrt2 + primaryCylinder;
    }

    static void daaNegativeYToMirror(float primaryCylinder, float secondaryCylinder, float *xForce,
                                     float *yForce, float *zForce) {
        *xForce = 0;
        *yForce = -secondaryCylinder * _reciprocalSqrt2;
        *zForce = secondaryCylinder * _reciprocalSqrt2 + primaryCylinder;
    }

    static void saaToMirror(float primaryCylinder, float secondaryCylinder, float *xForce, float *yForce,
                            float *zForce) {
        *xForce = 0;
        *yForce = 0;
        *zForce = primaryCylinder;
    }

private:
    static double constexpr _reciprocalSqrt2 = 0.70710678118654752440084436210485;
};

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */

#endif /* FORCECONVERTER_H_ */
