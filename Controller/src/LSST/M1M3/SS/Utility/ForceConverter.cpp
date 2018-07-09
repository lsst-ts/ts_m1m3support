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

DistributedForces ForceConverter::calculateForceDistribution(ForceActuatorSettings* forceActuatorSettings, float xForce, float yForce, float zForce, float xMoment, float yMoment, float zMoment) {
	DistributedForces forces;
	for(int i = 0; i < FA_COUNT; ++i) {
		forces.XForces[i] = 0;
		forces.YForces[i] = 0;
		forces.ZForces[i] = 0;
		forces.XForces[i] += forceActuatorSettings->ForceDistributionXTable[i * 3 + 0] * xForce;
		forces.YForces[i] += forceActuatorSettings->ForceDistributionXTable[i * 3 + 1] * xForce;
		forces.ZForces[i] += forceActuatorSettings->ForceDistributionXTable[i * 3 + 2] * xForce;
		forces.XForces[i] += forceActuatorSettings->ForceDistributionYTable[i * 3 + 0] * yForce;
		forces.YForces[i] += forceActuatorSettings->ForceDistributionYTable[i * 3 + 1] * yForce;
		forces.ZForces[i] += forceActuatorSettings->ForceDistributionYTable[i * 3 + 2] * yForce;
		forces.XForces[i] += forceActuatorSettings->ForceDistributionZTable[i * 3 + 0] * zForce;
		forces.YForces[i] += forceActuatorSettings->ForceDistributionZTable[i * 3 + 1] * zForce;
		forces.ZForces[i] += forceActuatorSettings->ForceDistributionZTable[i * 3 + 2] * zForce;
		// The moment table produces forces in millinewtons, need to multiply by 1000 to bring it up to newtons
		forces.XForces[i] += forceActuatorSettings->MomentDistributionXTable[i * 3 + 0] * xMoment * 1000.0;
		forces.YForces[i] += forceActuatorSettings->MomentDistributionXTable[i * 3 + 1] * xMoment * 1000.0;
		forces.ZForces[i] += forceActuatorSettings->MomentDistributionXTable[i * 3 + 2] * xMoment * 1000.0;
		forces.XForces[i] += forceActuatorSettings->MomentDistributionYTable[i * 3 + 0] * yMoment * 1000.0;
		forces.YForces[i] += forceActuatorSettings->MomentDistributionYTable[i * 3 + 1] * yMoment * 1000.0;
		forces.ZForces[i] += forceActuatorSettings->MomentDistributionYTable[i * 3 + 2] * yMoment * 1000.0;
		forces.XForces[i] += forceActuatorSettings->MomentDistributionZTable[i * 3 + 0] * zMoment * 1000.0;
		forces.YForces[i] += forceActuatorSettings->MomentDistributionZTable[i * 3 + 1] * zMoment * 1000.0;
		forces.ZForces[i] += forceActuatorSettings->MomentDistributionZTable[i * 3 + 2] * zMoment * 1000.0;
	}
	return forces;
}

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */
