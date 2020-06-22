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

#include <ForceConverter.h>
#include <ForceActuatorApplicationSettings.h>
#include <ForceActuatorSettings.h>

namespace LSST {
namespace M1M3 {
namespace SS {

ForcesAndMoments ForceConverter::calculateForcesAndMoments(
        ForceActuatorApplicationSettings* forceActuatorApplicationSettings,
        ForceActuatorSettings* forceActuatorSettings, float* xForces, float* yForces, float* zForces) {
    ForcesAndMoments fm;
    fm.Fx = 0;
    fm.Fy = 0;
    fm.Fz = 0;
    fm.Mx = 0;
    fm.My = 0;
    fm.Mz = 0;
    fm.ForceMagnitude = 0;
    for (int zIndex = 0; zIndex < FA_COUNT; ++zIndex) {
        int xIndex = forceActuatorApplicationSettings->ZIndexToXIndex[zIndex];
        int yIndex = forceActuatorApplicationSettings->ZIndexToYIndex[zIndex];
        float rx = forceActuatorApplicationSettings->Table[zIndex].XPosition -
                   forceActuatorSettings->MirrorCenterOfGravityX;
        float ry = forceActuatorApplicationSettings->Table[zIndex].YPosition -
                   forceActuatorSettings->MirrorCenterOfGravityY;
        float rz = forceActuatorApplicationSettings->Table[zIndex].ZPosition -
                   forceActuatorSettings->MirrorCenterOfGravityZ;
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

ForcesAndMoments ForceConverter::calculateForcesAndMoments(
        ForceActuatorApplicationSettings* forceActuatorApplicationSettings,
        ForceActuatorSettings* forceActuatorSettings, float* zForces) {
    ForcesAndMoments fm;
    fm.Fx = 0;
    fm.Fy = 0;
    fm.Fz = 0;
    fm.Mx = 0;
    fm.My = 0;
    fm.Mz = 0;
    fm.ForceMagnitude = 0;
    for (int zIndex = 0; zIndex < FA_COUNT; ++zIndex) {
        float rx = forceActuatorApplicationSettings->Table[zIndex].XPosition -
                   forceActuatorSettings->MirrorCenterOfGravityX;
        float ry = forceActuatorApplicationSettings->Table[zIndex].YPosition -
                   forceActuatorSettings->MirrorCenterOfGravityY;
        float rz = forceActuatorApplicationSettings->Table[zIndex].ZPosition -
                   forceActuatorSettings->MirrorCenterOfGravityZ;
        float fx = 0;
        float fy = 0;
        float fz = zForces[zIndex];

        fm.Fz += fz;
        fm.Mx += (fz * ry) - (fy * rz);
        fm.My += (fx * rz) - (fz * rx);
    }
    return fm;
}

DistributedForces ForceConverter::calculateForceFromBendingModes(ForceActuatorSettings* forceActuatorSettings,
                                                                 float* coefficients) {
    DistributedForces forces;
    for (int zIndex = 0; zIndex < FA_COUNT; ++zIndex) {
        int mIndex = zIndex * 22;
        forces.ZForces[zIndex] = forceActuatorSettings->BendingModeTable[mIndex + 0] * coefficients[0] +
                                 forceActuatorSettings->BendingModeTable[mIndex + 1] * coefficients[1] +
                                 forceActuatorSettings->BendingModeTable[mIndex + 2] * coefficients[2] +
                                 forceActuatorSettings->BendingModeTable[mIndex + 3] * coefficients[3] +
                                 forceActuatorSettings->BendingModeTable[mIndex + 4] * coefficients[4] +
                                 forceActuatorSettings->BendingModeTable[mIndex + 5] * coefficients[5] +
                                 forceActuatorSettings->BendingModeTable[mIndex + 6] * coefficients[6] +
                                 forceActuatorSettings->BendingModeTable[mIndex + 7] * coefficients[7] +
                                 forceActuatorSettings->BendingModeTable[mIndex + 8] * coefficients[8] +
                                 forceActuatorSettings->BendingModeTable[mIndex + 9] * coefficients[9] +
                                 forceActuatorSettings->BendingModeTable[mIndex + 10] * coefficients[10] +
                                 forceActuatorSettings->BendingModeTable[mIndex + 11] * coefficients[11] +
                                 forceActuatorSettings->BendingModeTable[mIndex + 12] * coefficients[12] +
                                 forceActuatorSettings->BendingModeTable[mIndex + 13] * coefficients[13] +
                                 forceActuatorSettings->BendingModeTable[mIndex + 14] * coefficients[14] +
                                 forceActuatorSettings->BendingModeTable[mIndex + 15] * coefficients[15] +
                                 forceActuatorSettings->BendingModeTable[mIndex + 16] * coefficients[16] +
                                 forceActuatorSettings->BendingModeTable[mIndex + 17] * coefficients[17] +
                                 forceActuatorSettings->BendingModeTable[mIndex + 18] * coefficients[18] +
                                 forceActuatorSettings->BendingModeTable[mIndex + 19] * coefficients[19] +
                                 forceActuatorSettings->BendingModeTable[mIndex + 20] * coefficients[20] +
                                 forceActuatorSettings->BendingModeTable[mIndex + 21] * coefficients[21];
    }
    return forces;
}

DistributedForces ForceConverter::calculateForceFromAngularAcceleration(
        ForceActuatorSettings* forceActuatorSettings, float angularAccelerationX, float angularAccelerationY,
        float angularAccelerationZ) {
    DistributedForces forces;
    for (int zIndex = 0; zIndex < FA_COUNT; ++zIndex) {
        int mIndex = zIndex * 3;

        forces.XForces[zIndex] =
                (forceActuatorSettings->AccelerationXTable[mIndex + 0] * angularAccelerationX +
                 forceActuatorSettings->AccelerationYTable[mIndex + 0] * angularAccelerationY +
                 forceActuatorSettings->AccelerationZTable[mIndex + 0] * angularAccelerationZ) /
                1000.0;

        forces.YForces[zIndex] =
                (forceActuatorSettings->AccelerationXTable[mIndex + 1] * angularAccelerationX +
                 forceActuatorSettings->AccelerationYTable[mIndex + 1] * angularAccelerationY +
                 forceActuatorSettings->AccelerationZTable[mIndex + 1] * angularAccelerationZ) /
                1000.0;

        forces.ZForces[zIndex] =
                (forceActuatorSettings->AccelerationXTable[mIndex + 2] * angularAccelerationX +
                 forceActuatorSettings->AccelerationYTable[mIndex + 2] * angularAccelerationY +
                 forceActuatorSettings->AccelerationZTable[mIndex + 2] * angularAccelerationZ) /
                1000.0;
    }

    return forces;
}

DistributedForces ForceConverter::calculateForceFromAngularVelocity(
        ForceActuatorSettings* forceActuatorSettings, float angularVelocityX, float angularVelocityY,
        float angularVelocityZ) {
    float angularVelocityXX = angularVelocityX * angularVelocityX;
    float angularVelocityYY = angularVelocityY * angularVelocityY;
    float angularVelocityZZ = angularVelocityZ * angularVelocityZ;
    float angularVelocityXZ = angularVelocityX * angularVelocityZ;
    float angularVelocityYZ = angularVelocityY * angularVelocityZ;
    DistributedForces forces;
    for (int zIndex = 0; zIndex < FA_COUNT; ++zIndex) {
        int mIndex = zIndex * 3;

        forces.XForces[zIndex] = (forceActuatorSettings->VelocityXTable[mIndex + 0] * angularVelocityXX +
                                  forceActuatorSettings->VelocityYTable[mIndex + 0] * angularVelocityYY +
                                  forceActuatorSettings->VelocityZTable[mIndex + 0] * angularVelocityZZ +
                                  forceActuatorSettings->VelocityXZTable[mIndex + 0] * angularVelocityXZ +
                                  forceActuatorSettings->VelocityYZTable[mIndex + 0] * angularVelocityYZ) /
                                 1000.0;

        forces.YForces[zIndex] = (forceActuatorSettings->VelocityXTable[mIndex + 1] * angularVelocityXX +
                                  forceActuatorSettings->VelocityYTable[mIndex + 1] * angularVelocityYY +
                                  forceActuatorSettings->VelocityZTable[mIndex + 1] * angularVelocityZZ +
                                  forceActuatorSettings->VelocityXZTable[mIndex + 1] * angularVelocityXZ +
                                  forceActuatorSettings->VelocityYZTable[mIndex + 1] * angularVelocityYZ) /
                                 1000.0;

        forces.ZForces[zIndex] = (forceActuatorSettings->VelocityXTable[mIndex + 2] * angularVelocityXX +
                                  forceActuatorSettings->VelocityYTable[mIndex + 2] * angularVelocityYY +
                                  forceActuatorSettings->VelocityZTable[mIndex + 2] * angularVelocityZZ +
                                  forceActuatorSettings->VelocityXZTable[mIndex + 2] * angularVelocityXZ +
                                  forceActuatorSettings->VelocityYZTable[mIndex + 2] * angularVelocityYZ) /
                                 1000.0;
    }

    return forces;
}

DistributedForces ForceConverter::calculateForceFromAzimuthAngle(ForceActuatorSettings* forceActuatorSettings,
                                                                 float azimuthAngle) {
    float azimuthMatrix[] = {std::pow(azimuthAngle, 5.0f),
                             std::pow(azimuthAngle, 4.0f),
                             std::pow(azimuthAngle, 3.0f),
                             std::pow(azimuthAngle, 2.0f),
                             azimuthAngle,
                             1.0f};
    DistributedForces forces;
    for (int zIndex = 0; zIndex < FA_COUNT; ++zIndex) {
        int mIndex = zIndex * 6;
        forces.XForces[zIndex] = forceActuatorSettings->AzimuthXTable[mIndex + 0] * azimuthMatrix[0] +
                                 forceActuatorSettings->AzimuthXTable[mIndex + 1] * azimuthMatrix[1] +
                                 forceActuatorSettings->AzimuthXTable[mIndex + 2] * azimuthMatrix[2] +
                                 forceActuatorSettings->AzimuthXTable[mIndex + 3] * azimuthMatrix[3] +
                                 forceActuatorSettings->AzimuthXTable[mIndex + 4] * azimuthMatrix[4] +
                                 forceActuatorSettings->AzimuthXTable[mIndex + 5];

        forces.YForces[zIndex] = forceActuatorSettings->AzimuthYTable[mIndex + 0] * azimuthMatrix[0] +
                                 forceActuatorSettings->AzimuthYTable[mIndex + 1] * azimuthMatrix[1] +
                                 forceActuatorSettings->AzimuthYTable[mIndex + 2] * azimuthMatrix[2] +
                                 forceActuatorSettings->AzimuthYTable[mIndex + 3] * azimuthMatrix[3] +
                                 forceActuatorSettings->AzimuthYTable[mIndex + 4] * azimuthMatrix[4] +
                                 forceActuatorSettings->AzimuthYTable[mIndex + 5];

        forces.ZForces[zIndex] = forceActuatorSettings->AzimuthZTable[mIndex + 0] * azimuthMatrix[0] +
                                 forceActuatorSettings->AzimuthZTable[mIndex + 1] * azimuthMatrix[1] +
                                 forceActuatorSettings->AzimuthZTable[mIndex + 2] * azimuthMatrix[2] +
                                 forceActuatorSettings->AzimuthZTable[mIndex + 3] * azimuthMatrix[3] +
                                 forceActuatorSettings->AzimuthZTable[mIndex + 4] * azimuthMatrix[4] +
                                 forceActuatorSettings->AzimuthZTable[mIndex + 5];
    }
    return forces;
}

DistributedForces ForceConverter::calculateForceFromElevationAngle(
        ForceActuatorSettings* forceActuatorSettings, float elevationAngle) {
    float elevationMatrix[] = {std::pow(elevationAngle, 5.0f),
                               std::pow(elevationAngle, 4.0f),
                               std::pow(elevationAngle, 3.0f),
                               std::pow(elevationAngle, 2.0f),
                               elevationAngle,
                               1.0f};
    DistributedForces forces;
    for (int zIndex = 0; zIndex < FA_COUNT; ++zIndex) {
        int mIndex = zIndex * 6;
        forces.XForces[zIndex] = forceActuatorSettings->ElevationXTable[mIndex + 0] * elevationMatrix[0] +
                                 forceActuatorSettings->ElevationXTable[mIndex + 1] * elevationMatrix[1] +
                                 forceActuatorSettings->ElevationXTable[mIndex + 2] * elevationMatrix[2] +
                                 forceActuatorSettings->ElevationXTable[mIndex + 3] * elevationMatrix[3] +
                                 forceActuatorSettings->ElevationXTable[mIndex + 4] * elevationMatrix[4] +
                                 forceActuatorSettings->ElevationXTable[mIndex + 5];

        forces.YForces[zIndex] = forceActuatorSettings->ElevationYTable[mIndex + 0] * elevationMatrix[0] +
                                 forceActuatorSettings->ElevationYTable[mIndex + 1] * elevationMatrix[1] +
                                 forceActuatorSettings->ElevationYTable[mIndex + 2] * elevationMatrix[2] +
                                 forceActuatorSettings->ElevationYTable[mIndex + 3] * elevationMatrix[3] +
                                 forceActuatorSettings->ElevationYTable[mIndex + 4] * elevationMatrix[4] +
                                 forceActuatorSettings->ElevationYTable[mIndex + 5];

        forces.ZForces[zIndex] = forceActuatorSettings->ElevationZTable[mIndex + 0] * elevationMatrix[0] +
                                 forceActuatorSettings->ElevationZTable[mIndex + 1] * elevationMatrix[1] +
                                 forceActuatorSettings->ElevationZTable[mIndex + 2] * elevationMatrix[2] +
                                 forceActuatorSettings->ElevationZTable[mIndex + 3] * elevationMatrix[3] +
                                 forceActuatorSettings->ElevationZTable[mIndex + 4] * elevationMatrix[4] +
                                 forceActuatorSettings->ElevationZTable[mIndex + 5];
    }
    return forces;
}

DistributedForces ForceConverter::calculateForceFromTemperature(ForceActuatorSettings* forceActuatorSettings,
                                                                float temperature) {
    float temperatureMatrix[] = {std::pow(temperature, 5.0f),
                                 std::pow(temperature, 4.0f),
                                 std::pow(temperature, 3.0f),
                                 std::pow(temperature, 2.0f),
                                 temperature,
                                 1.0f};
    DistributedForces forces;
    for (int zIndex = 0; zIndex < FA_COUNT; ++zIndex) {
        int mIndex = zIndex * 6;
        forces.XForces[zIndex] = forceActuatorSettings->ThermalXTable[mIndex + 0] * temperatureMatrix[0] +
                                 forceActuatorSettings->ThermalXTable[mIndex + 1] * temperatureMatrix[1] +
                                 forceActuatorSettings->ThermalXTable[mIndex + 2] * temperatureMatrix[2] +
                                 forceActuatorSettings->ThermalXTable[mIndex + 3] * temperatureMatrix[3] +
                                 forceActuatorSettings->ThermalXTable[mIndex + 4] * temperatureMatrix[4] +
                                 forceActuatorSettings->ThermalXTable[mIndex + 5];

        forces.YForces[zIndex] = forceActuatorSettings->ThermalYTable[mIndex + 0] * temperatureMatrix[0] +
                                 forceActuatorSettings->ThermalYTable[mIndex + 1] * temperatureMatrix[1] +
                                 forceActuatorSettings->ThermalYTable[mIndex + 2] * temperatureMatrix[2] +
                                 forceActuatorSettings->ThermalYTable[mIndex + 3] * temperatureMatrix[3] +
                                 forceActuatorSettings->ThermalYTable[mIndex + 4] * temperatureMatrix[4] +
                                 forceActuatorSettings->ThermalYTable[mIndex + 5];

        forces.ZForces[zIndex] = forceActuatorSettings->ThermalZTable[mIndex + 0] * temperatureMatrix[0] +
                                 forceActuatorSettings->ThermalZTable[mIndex + 1] * temperatureMatrix[1] +
                                 forceActuatorSettings->ThermalZTable[mIndex + 2] * temperatureMatrix[2] +
                                 forceActuatorSettings->ThermalZTable[mIndex + 3] * temperatureMatrix[3] +
                                 forceActuatorSettings->ThermalZTable[mIndex + 4] * temperatureMatrix[4] +
                                 forceActuatorSettings->ThermalZTable[mIndex + 5];
    }
    return forces;
}

DistributedForces ForceConverter::calculateForceDistribution(ForceActuatorSettings* forceActuatorSettings,
                                                             float xForce, float yForce, float zForce,
                                                             float xMoment, float yMoment, float zMoment) {
    DistributedForces forces;
    for (int i = 0; i < FA_COUNT; ++i) {
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
        // The moment table produces forces in millinewtons, need to multiply by 1000 to bring it up to
        // newtons
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
