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
    void _sumAllForces();
    void _convertForcesToSetpoints();

    bool _checkMirrorMoments();
    bool _checkNearNeighbors();
    bool _checkMirrorWeight();
    bool _checkFarNeighbors();

    static double constexpr _sqrt2 = 1.4142135623730950488016887242097;

    ForceActuatorApplicationSettings* _forceActuatorApplicationSettings;
    ForceActuatorSettings* _forceActuatorSettings;
    PIDSettings* _pidSettings;
    M1M3SSPublisher* _publisher;
    SafetyController* _safetyController;

    AberrationForceComponent _aberrationForceComponent;
    AccelerationForceComponent _accelerationForceComponent;
    ActiveOpticForceComponent _activeOpticForceComponent;
    AzimuthForceComponent _azimuthForceComponent;
    BalanceForceComponent _balanceForceComponent;
    ElevationForceComponent _elevationForceComponent;
    OffsetForceComponent _offsetForceComponent;
    StaticForceComponent _staticForceComponent;
    ThermalForceComponent _thermalForceComponent;
    VelocityForceComponent _velocityForceComponent;
    FinalForceComponent _finalForceComponent;

    MTM1M3_logevent_appliedCylinderForcesC* _appliedCylinderForces;
    MTM1M3_logevent_appliedForcesC* _appliedForces;
    MTM1M3_logevent_forceActuatorStateC* _forceActuatorState;
    MTM1M3_logevent_forceSetpointWarningC* _forceSetpointWarning;
    MTM1M3_logevent_rejectedCylinderForcesC* _rejectedCylinderForces;

    MTM1M3_logevent_forceActuatorInfoC* _forceActuatorInfo;
    MTM1M3_forceActuatorDataC* _forceActuatorData;

    MTM1M3_inclinometerDataC* _inclinometerData;
    MTM1M3_pidDataC* _pidData;
    MTM1M3_logevent_pidInfoC* _pidInfo;
    MTM1M3_hardpointActuatorDataC* _hardpointActuatorData;
    MTM1M3_accelerometerDataC* _accelerometerData;
    MTM1M3_gyroDataC* _gyroData;

    double _elevation_Timestamp;
    double _elevation_Angle_Actual;

    std::vector<ForceActuatorNeighbors> _neighbors;

    float _zero[156];
    float _mirrorWeight;

    static int32_t _toInt24(float force) { return (int32_t)(force * 1000.0); }
};

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */

#endif /* FORCECONTROLLER_H_ */
