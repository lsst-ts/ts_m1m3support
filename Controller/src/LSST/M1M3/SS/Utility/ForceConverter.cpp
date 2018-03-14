/*
 * ForceConverter.cpp
 *
 *  Created on: Oct 20, 2017
 *      Author: ccontaxis
 */

#include <ForceConverter.h>
#include <ForceActuatorApplicationSettings.h>
#include <ForceActuatorSettings.h>

namespace LSST {
namespace M1M3 {
namespace SS {

ForcesAndMoments ForceConverter::calculateForcesAndMoments(ForceActuatorApplicationSettings* forceActuatorApplicationSettings, ForceActuatorSettings* forceActuatorSettings, float* xForces, float* yForces, float* zForces) {
	ForcesAndMoments fm;
	fm.Fx = 0;
	fm.Fy = 0;
	fm.Fz = 0;
	fm.Mx = 0;
	fm.My = 0;
	fm.Mz = 0;
	fm.ForceMagnitude = 0;
	for(int zIndex = 0; zIndex < FA_COUNT; ++zIndex) {
		int xIndex = forceActuatorApplicationSettings->ZIndexToXIndex[zIndex];
		int yIndex = forceActuatorApplicationSettings->ZIndexToYIndex[zIndex];
		float rx = forceActuatorApplicationSettings->Table[zIndex].XPosition - forceActuatorSettings->MirrorCenterOfGravityX;
		float ry = forceActuatorApplicationSettings->Table[zIndex].YPosition - forceActuatorSettings->MirrorCenterOfGravityY;
		float rz = forceActuatorApplicationSettings->Table[zIndex].ZPosition - forceActuatorSettings->MirrorCenterOfGravityZ;
		float fx = 0;
		float fy = 0;
		float fz = zForces[zIndex];

		if (xIndex != -1) {
			fx = xForces[xIndex];
		}

		if (yIndex != -1) {
			fy = yForces[yIndex];
		}

		fm.Fx += fx;
		fm.Fy += fy;
		fm.Fz += fz;
		fm.Mx += (fz * ry) - (fy * rz);
		fm.My += (fx * rz) - (fz * rx);
		fm.Mz += (fy * rx) - (fx * ry);
	}
	fm.ForceMagnitude = sqrt(fm.Fx * fm.Fx + fm.Fy * fm.Fy + fm.Fz * fm.Fz);
	return fm;
}

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */
