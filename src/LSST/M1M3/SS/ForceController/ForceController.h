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

#ifndef FORCECONTROLLER_H_
#define FORCECONTROLLER_H_

#include <DataTypes.h>
#include <ForceActuatorNeighbors.h>
#include <ForcesAndMoments.h>
#include <SAL_MTM1M3C.h>
#include <SAL_MTMountC.h>
#include <DistributedForces.h>
#include <PID.h>
#include <vector>
#include <AberrationForceComponent.h>
#include <AccelerationForceComponent.h>
#include <ActiveOpticForceComponent.h>
#include <AzimuthForceComponent.h>
#include <BalanceForceComponent.h>
#include <ElevationForceComponent.h>
#include <OffsetForceComponent.h>
#include <StaticForceComponent.h>
#include <ThermalForceComponent.h>
#include <VelocityForceComponent.h>
#include <FinalForceComponent.h>

namespace LSST {
namespace M1M3 {
namespace SS {

class ForceActuatorApplicationSettings;
class ForceActuatorSettings;
class M1M3SSPublisher;
class SafetyController;
class PIDSettings;

class ForceController {
public:
    ForceController(ForceActuatorApplicationSettings* forceActuatorApplicationSettings,
                    ForceActuatorSettings* forceActuatorSettings, PIDSettings* pidSettings,
                    M1M3SSPublisher* publisher, SafetyController* safetyController);

    void reset();

    void updateTMAElevationData(MTMount_ElevationC* tmaElevationData);

    void incSupportPercentage();
    void decSupportPercentage();
    void zeroSupportPercentage();
    void fillSupportPercentage();
    bool supportPercentageFilled();
    bool supportPercentageZeroed();
    bool followingErrorInTolerance();

    void updateAppliedForces();
    void processAppliedForces();

    void applyAberrationForcesByBendingModes(float* coefficients);
    void applyAberrationForces(float* z);
    void zeroAberrationForces();

    void applyAccelerationForces();
    void zeroAccelerationForces();

    void applyActiveOpticForcesByBendingModes(float* coefficients);
    void applyActiveOpticForces(float* z);
    void zeroActiveOpticForces();

    void applyAzimuthForces();
    void updateAzimuthForces(float azimuthAngle);
    void zeroAzimuthForces();

    void applyBalanceForces();
    void zeroBalanceForces();
    void updatePID(int id, PIDParameters parameters);
    void resetPID(int id);
    void resetPIDs();

    void applyElevationForces();
    void zeroElevationForces();

    void applyOffsetForces(float* x, float* y, float* z);
    void applyOffsetForcesByMirrorForces(float xForce, float yForce, float zForce, float xMoment,
                                         float yMoment, float zMoment);
    void zeroOffsetForces();

    void applyStaticForces();
    void zeroStaticForces();

    void applyThermalForces();
    void updateThermalForces(float temperature);
    void zeroThermalForces();

    void applyVelocityForces();
    void zeroVelocityForces();

private:
    void sumAllForces();
    void convertForcesToSetpoints();

    bool checkMirrorMoments();
    bool checkNearNeighbors();
    bool checkMirrorWeight();
    bool checkFarNeighbors();

    static double constexpr sqrt2 = 1.4142135623730950488016887242097;
    static double constexpr reciprocalSqrt2 = 0.70710678118654752440084436210485;

    ForceActuatorApplicationSettings* forceActuatorApplicationSettings;
    ForceActuatorSettings* forceActuatorSettings;
    PIDSettings* pidSettings;
    M1M3SSPublisher* publisher;
    SafetyController* safetyController;

    AberrationForceComponent aberrationForceComponent;
    AccelerationForceComponent accelerationForceComponent;
    ActiveOpticForceComponent activeOpticForceComponent;
    AzimuthForceComponent azimuthForceComponent;
    BalanceForceComponent balanceForceComponent;
    ElevationForceComponent elevationForceComponent;
    OffsetForceComponent offsetForceComponent;
    StaticForceComponent staticForceComponent;
    ThermalForceComponent thermalForceComponent;
    VelocityForceComponent velocityForceComponent;
    FinalForceComponent finalForceComponent;

    MTM1M3_logevent_appliedCylinderForcesC* appliedCylinderForces;
    MTM1M3_logevent_appliedForcesC* appliedForces;
    MTM1M3_logevent_forceActuatorStateC* forceActuatorState;
    MTM1M3_logevent_forceSetpointWarningC* forceSetpointWarning;
    MTM1M3_logevent_rejectedCylinderForcesC* rejectedCylinderForces;

    MTM1M3_logevent_forceActuatorInfoC* forceActuatorInfo;
    MTM1M3_forceActuatorDataC* forceActuatorData;

    MTM1M3_inclinometerDataC* inclinometerData;
    MTM1M3_pidDataC* pidData;
    MTM1M3_logevent_pidInfoC* pidInfo;
    MTM1M3_hardpointActuatorDataC* hardpointActuatorData;
    MTM1M3_accelerometerDataC* accelerometerData;
    MTM1M3_gyroDataC* gyroData;

    double elevation_Timestamp;
    double elevation_Angle_Actual;

    std::vector<ForceActuatorNeighbors> neighbors;

    float zero[156];
    float mirrorWeight;

    static int32_t toInt24(float force) { return (int32_t)(force * 1000.0); }
};

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */

#endif /* FORCECONTROLLER_H_ */
