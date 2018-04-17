/*
 * ForceConverter.h
 *
 *  Created on: Oct 20, 2017
 *      Author: ccontaxis
 */

#ifndef FORCECONVERTER_H_
#define FORCECONVERTER_H_

#include <DataTypes.h>
#include <cmath>
#include <ForcesAndMoments.h>

namespace LSST {
namespace M1M3 {
namespace SS {

class ForceActuatorApplicationSettings;
class ForceActuatorSettings;

class ForceConverter {
private:
	static double const sqrt2 = 1.4142135623730950488016887242097;
	static double const reciprocalSqrt2 = 0.70710678118654752440084436210485;

public:
	static void daaPositiveXToMirror(float primaryCylinder, float secondaryCylinder, float* xForce, float* yForce, float* zForce) {
		*xForce = secondaryCylinder * reciprocalSqrt2;
		*yForce = 0;
		*zForce = secondaryCylinder * reciprocalSqrt2 + primaryCylinder;
	}

	static void daaNegativeXToMirror(float primaryCylinder, float secondaryCylinder, float* xForce, float* yForce, float* zForce) {
		*xForce = -secondaryCylinder * reciprocalSqrt2;
		*yForce = 0;
		*zForce = secondaryCylinder * reciprocalSqrt2 + primaryCylinder;
	}

	static void daaPositiveYToMirror(float primaryCylinder, float secondaryCylinder, float* xForce, float* yForce, float* zForce) {
		*xForce = 0;
		*yForce = secondaryCylinder * reciprocalSqrt2;
		*zForce = secondaryCylinder * reciprocalSqrt2 + primaryCylinder;
	}

	static void daaNegativeYToMirror(float primaryCylinder, float secondaryCylinder, float* xForce, float* yForce, float* zForce) {
		*xForce = 0;
		*yForce = -secondaryCylinder * reciprocalSqrt2;
		*zForce = secondaryCylinder * reciprocalSqrt2 + primaryCylinder;
	}

	static void saaToMirror(float primaryCylinder, float secondaryCylinder, float* xForce, float* yForce, float* zForce) {
		*xForce = 0;
		*yForce = 0;
		*zForce = primaryCylinder;
	}

	static ForcesAndMoments calculateForcesAndMoments(ForceActuatorApplicationSettings* forceActuatorApplicationSettings, ForceActuatorSettings* forceActuatorSettings, float* xForces, float* yForces, float* zForces);

};

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */

#endif /* FORCECONVERTER_H_ */
