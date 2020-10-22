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
#include <ForceActuatorApplicationSettings.h>
#include <ForceActuatorSettings.h>
#include <SafetyController.h>
#include <PIDSettings.h>

namespace LSST {
namespace M1M3 {
namespace SS {

/**
 * Coordinate force actuators force calculcation. The mirror weight and
 * external forces acting on the mirror shall be counteracted by the force
 * actuators. ForceController coordinates this - calculates contribution of the
 * mirror weight and external forces (acceleration,..), distributes this
 * between the force actuators. It also check for the mirror safety - runs
 * various checks on the computed forces to confirm mirror stress stays within
 * the mirror stress limits.
 *
 * ## Mirror operation
 *
 * Without any power and active corrections, M1M3 mirror is resting on the
 * passive supports (coil springs). When the mirror shall be operated, it needs
 * to be raised on active force controllers above the passive support. Rainsing
 * of the mirror needs to be done in steps, progressing from passive support
 * towards fully active support.
 *
 * Mirror weight (gravitational force, which depends on elevation, so is
 * calculated by ElevationForceComponent) can be supported (counteracted) from
 * 0 to 100%. Mirror is active when its weight is fully supported by actuators.
 * See incSupportPercentage(), decSupportPercentage(), zeroSupportPercentage()
 * and fillSupportPercentage() methods for support percentage manipulation. As
 * the mirror is being raised, the support fraction is increased. When the
 * mirror is being lowered, its support fraction is lowered.
 *
 * The supportPercentageFilled() and supportPercentageZeroed() methods reports
 * mirror raising state.
 *
 * Actual support percentage, together with the force actuator state and
 * enabled ForceComponent subclass, is reported in forceActuatorState event.
 *
 * Various other corrections - manual offsets, dynamic force (acceleration when
 * TMA is moving), bending mode corrections for active optics - are handled in
 * dedicated ForceComponent subclass. Those are enabled only if mirror is
 * active, fully supported against gravity (handled by ElevationForceComponent).
 *
 * ## Distribution of mirror forces
 *
 * Calculated force demands are stored in preclipped* events
 * (prelippedElevationForces,..). Those are clip into allowed range, and the
 * resulting force is stored in applied* (appliedElevationForces,..) events.
 *
 * The algorithm tries to prevent large changes in applied forces. If a large
 * change is detected, the change is distributed to multiple steps. After their
 * execution, mirror shall end in requested position and shape.
 *
 * The algorithm tries to do its best to prevent mirror panic due to violation
 * of stress limits. Stress limits are then checked, and if violation is
 * detected, mirror is send to State::LoweringFaultState (panic mode).
 *
 * ## Mirror safety
 *
 * Following checks are run:
 *
 * * Mirror moments - _checkMirrorMoments
 * * Near neighbors - _checkNearNeighbors
 * * Mirror weight - _checkMirrorWeight
 * * Far neighbors - _checkFarNeighbors
 *
 * Please see LBTO UA 95-02 document for description of those checks (Chapter 9
 * [Mirror Support Safety], Subchapter 9.2 [Methods of Protection]).
 *
 * The checks are run from ForceController::processAppliedForces() method.
 *
 * Check results are reported to SafetyController. Any violation of the M1M3
 * limits leads to SafetyController::checkSafety() returning
 * States::LoweringFaultState as next M1M3. That forces M1M3 to panic and go to
 * passive supports, as violations aren't allowed.
 */
class ForceController {
public:
    ForceController(ForceActuatorApplicationSettings* forceActuatorApplicationSettings,
                    ForceActuatorSettings* forceActuatorSettings, PIDSettings* pidSettings,
                    SafetyController* safetyController);

    void reset();

    /**
     * Updates elevation data.
     *
     * @param tmaElevationData
     */
    void updateTMAElevationData(MTMount_ElevationC* tmaElevationData);

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
    bool followingErrorInTolerance();

    void updateAppliedForces();

    /**
     * @brief Sums components and run mirror safety checks.
     *
     * Uses FinalForceComponent to sum mirror forces obtained from
     * AberrationForceComponent, AccelerationForceComponent,
     * ActiveOpticForceComponent, AzimuthForceComponent, BalanceForceComponent,
     * ElevationForceComponent, OffsetForceComponent, StaticForceComponent,
     * ThermalForceComponent and VelocityForceComponent. Run mirror safety
     * checks on summed forces, and log warning if forces are clipped.
     */
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
    MTM1M3_logevent_preclippedCylinderForcesC* _preclippedCylinderForces;

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

    float _zero[FA_COUNT];
    float _mirrorWeight;

    static int32_t _toInt24(float force) { return (int32_t)(force * 1000.0); }
};

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */

#endif /* FORCECONTROLLER_H_ */
