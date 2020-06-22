#ifndef FORCECONVERTER_H_
#define FORCECONVERTER_H_

#include <DataTypes.h>
#include <cmath>
#include <ForcesAndMoments.h>
#include <DistributedForces.h>

namespace LSST {
namespace M1M3 {
namespace SS {

class ForceActuatorApplicationSettings;
class ForceActuatorSettings;

class ForceConverter {
private:
    static double constexpr sqrt2 = 1.4142135623730950488016887242097;
    static double constexpr reciprocalSqrt2 = 0.70710678118654752440084436210485;

public:
    static void daaPositiveXToMirror(float primaryCylinder, float secondaryCylinder, float* xForce,
                                     float* yForce, float* zForce) {
        *xForce = secondaryCylinder * reciprocalSqrt2;
        *yForce = 0;
        *zForce = secondaryCylinder * reciprocalSqrt2 + primaryCylinder;
    }

    static void daaNegativeXToMirror(float primaryCylinder, float secondaryCylinder, float* xForce,
                                     float* yForce, float* zForce) {
        *xForce = -secondaryCylinder * reciprocalSqrt2;
        *yForce = 0;
        *zForce = secondaryCylinder * reciprocalSqrt2 + primaryCylinder;
    }

    static void daaPositiveYToMirror(float primaryCylinder, float secondaryCylinder, float* xForce,
                                     float* yForce, float* zForce) {
        *xForce = 0;
        *yForce = secondaryCylinder * reciprocalSqrt2;
        *zForce = secondaryCylinder * reciprocalSqrt2 + primaryCylinder;
    }

    static void daaNegativeYToMirror(float primaryCylinder, float secondaryCylinder, float* xForce,
                                     float* yForce, float* zForce) {
        *xForce = 0;
        *yForce = -secondaryCylinder * reciprocalSqrt2;
        *zForce = secondaryCylinder * reciprocalSqrt2 + primaryCylinder;
    }

    static void saaToMirror(float primaryCylinder, float secondaryCylinder, float* xForce, float* yForce,
                            float* zForce) {
        *xForce = 0;
        *yForce = 0;
        *zForce = primaryCylinder;
    }

    static ForcesAndMoments calculateForcesAndMoments(
            ForceActuatorApplicationSettings* forceActuatorApplicationSettings,
            ForceActuatorSettings* forceActuatorSettings, float* xForces, float* yForces, float* zForces);
    static ForcesAndMoments calculateForcesAndMoments(
            ForceActuatorApplicationSettings* forceActuatorApplicationSettings,
            ForceActuatorSettings* forceActuatorSettings, float* zForces);

    static DistributedForces calculateForceFromBendingModes(ForceActuatorSettings* forceActuatorSettings,
                                                            float* coefficients);
    static DistributedForces calculateForceFromAngularAcceleration(
            ForceActuatorSettings* forceActuatorSettings, float angularAccelerationX,
            float angularAccelerationY, float angularAccelerationZ);
    static DistributedForces calculateForceFromAngularVelocity(ForceActuatorSettings* forceActuatorSettings,
                                                               float angularVelocityX, float angularVelocityY,
                                                               float angularVelocityZ);
    static DistributedForces calculateForceFromAzimuthAngle(ForceActuatorSettings* forceActuatorSettings,
                                                            float azimuthAngle);
    static DistributedForces calculateForceFromElevationAngle(ForceActuatorSettings* forceActuatorSettings,
                                                              float elevationAngle);
    static DistributedForces calculateForceFromTemperature(ForceActuatorSettings* forceActuatorSettings,
                                                           float temperature);
    static DistributedForces calculateForceDistribution(ForceActuatorSettings* forceActuatorSettings,
                                                        float xForce, float yForce, float zForce,
                                                        float xMoment, float yMoment, float zMoment);
};

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */

#endif /* FORCECONVERTER_H_ */
