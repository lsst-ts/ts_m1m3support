/*
 * This file is part of LSST M1M3 support system package.
 *
 * Developed for the LSST Telescope & Site Software Systems.
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

#include <spdlog/spdlog.h>
#include <vector>

#include <SAL_MTM1M3C.h>
#include <SAL_MTMountC.h>

#include <AccelerationForceComponent.h>
#include <ActiveOpticForceComponent.h>
#include <AzimuthForceComponent.h>
#include <BalanceForceComponent.h>
#include <DataTypes.h>
#include <DistributedForces.h>
#include <ElevationForceComponent.h>
#include <FinalForceComponent.h>
#include <ForceActuatorApplicationSettings.h>
#include <ForceActuatorSettings.h>
#include <ForcesAndMoments.h>
#include <LimitTrigger.h>
#include <OffsetForceComponent.h>
#include <PID.h>
#include <SafetyController.h>
#include <StaticForceComponent.h>
#include <ThermalForceComponent.h>
#include <VelocityForceComponent.h>

namespace LSST {
namespace M1M3 {
namespace SS {

struct ForceActuatorIndicesNeighbors {
    ForceActuatorIndicesNeighbors();
    int nearCount;
    int NearZIndices[FA_MAX_NEAR_COUNT];
    int FarIndices[FA_FAR_COUNT];
};

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
 * See RaisingLoweringInfo for methods to manipulate the support percentage. As
 * the mirror is being raised, the support fraction is increased. When the
 * mirror is being lowered, its support fraction is lowered.
 *
 * The force actuator state and enabled ForceComponent subclass is reported in
 * forceActuatorState event.
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
    ForceController(ForceActuatorApplicationSettings* forceActuatorApplicationSettings);

    void reset();

    /**
     * Updates elevation data.
     *
     * @param tmaElevationData
     */
    void updateTMAElevationData(MTMount_elevationC* tmaElevationData);

    /**
     * Tests following error on all actuaturs. Reports any violation into
     * spdlog.
     */
    bool faRaiseFollowingErrorInTolerance();

    void updateAppliedForces();

    /**
     * @brief Sums components and run mirror safety checks.
     *
     * Uses FinalForceComponent to sum mirror forces obtained from
     * AccelerationForceComponent,
     * ActiveOpticForceComponent, AzimuthForceComponent, BalanceForceComponent,
     * ElevationForceComponent, OffsetForceComponent, StaticForceComponent,
     * ThermalForceComponent and VelocityForceComponent. Run mirror safety
     * checks on summed forces, and log warning if forces are clipped.
     */
    void processAppliedForces();

    /**
     * Apply acceleration forces
     *
     * @return true if acceleration forces were already enabled
     */
    bool applyAccelerationForces();
    void zeroAccelerationForces();

    void applyActiveOpticForces(std::vector<float> z);
    void zeroActiveOpticForces();

    void applyAzimuthForces();
    void updateTMAAzimuthForces(MTMount_azimuthC* tmaAzimuthData);
    void zeroAzimuthForces();

    bool applyBalanceForces();
    bool zeroBalanceForces();
    void updatePID(int id, PIDParameters parameters);

    /**
     * Reset given PID settings to system default (tracking).
     *
     * @param id PID id (0 based, 0-5)
     */
    void resetPID(int id);

    /**
     * Resets all PIDs to initial (tracking) settings.
     */
    void resetPIDs();

    void applyElevationForces();
    void zeroElevationForces();

    void applyOffsetForces(std::vector<float> x, std::vector<float> y, std::vector<float> z);
    void applyOffsetForcesByMirrorForces(float xForce, float yForce, float zForce, float xMoment,
                                         float yMoment, float zMoment);
    void zeroOffsetForces();

    void applyActuatorOffset(char axis, int index, float offset);

    void applyStaticForces();
    void zeroStaticForces();

    void applyThermalForces();
    void updateThermalForces(float temperature);
    void zeroThermalForces();

    /**
     * Apply velocity forces.
     *
     * @return true if velocity forces were already enabled
     */
    bool applyVelocityForces();
    void zeroVelocityForces();

    void enableDisableForceComponent(int forceComponentEnum, bool enabled);

private:
    void _sumAllForces();
    void _convertForcesToSetpoints();

    bool _checkMirrorMoments();
    bool _checkNearNeighbors();
    bool _checkMirrorWeight();
    bool _checkFarNeighbors();

    static double constexpr _sqrt2 = 1.4142135623730950488016887242097;

    ForceActuatorApplicationSettings* _forceActuatorApplicationSettings;
    SafetyController* _safetyController;

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

    MTM1M3_appliedCylinderForcesC* _appliedCylinderForces;
    MTM1M3_appliedForcesC* _appliedForces;
    MTM1M3_logevent_forceActuatorStateC* _forceActuatorState;
    MTM1M3_logevent_forceSetpointWarningC* _forceSetpointWarning;
    MTM1M3_logevent_preclippedCylinderForcesC* _preclippedCylinderForces;

    MTM1M3_inclinometerDataC* _inclinometerData;
    MTM1M3_pidDataC* _pidData;
    MTM1M3_logevent_pidInfoC* _pidInfo;
    MTM1M3_hardpointActuatorDataC* _hardpointActuatorData;
    MTM1M3_accelerometerDataC* _accelerometerData;
    MTM1M3_gyroDataC* _gyroData;

    ForceActuatorIndicesNeighbors _neighbors[FA_COUNT];

    float _zero[FA_COUNT];
    float _mirrorWeight;

    class ForceLimitTrigger : public LimitTrigger<float, float> {
    public:
        ForceLimitTrigger() {
            _axis = 'U';
            _faId = -1;
            _counter = 0;
        }

        ForceLimitTrigger(char axis, int faId) {
            _axis = axis;
            _faId = faId;
            _counter = 0;
        }

    protected:
        bool trigger() override {
            _counter++;
            // problems are reported  when accumulated counter reaches those levels
            static int levels[4] = {1, 200, 500, 100000};
            for (int i = 0; i < 4; i++)
                if (levels[i] == _counter) return true;
            return ((_counter % levels[3]) == 0);
        }

        void execute(float limit, float fe) override {
            SPDLOG_WARN("Violated {} follow-up error FA ID {} measured error {} ({}th occurence), limit +-{}",
                        _axis, _faId, fe, _counter, limit);
        }

        void reset() override {
            if (_counter > 0) {
                SPDLOG_INFO("FA ID {} axis {} following error is back into limits after {} failures", _faId,
                            _axis, _counter);
                _counter = 0;
            }
        }

    private:
        char _axis;
        int _faId;
        int _counter;
    };

    ForceLimitTrigger limitTriggerX[FA_X_COUNT];
    ForceLimitTrigger limitTriggerY[FA_Y_COUNT];
    ForceLimitTrigger limitTriggerZ[FA_Z_COUNT];

    static int32_t _toInt24(float force) { return (int32_t)(force * 1000.0); }
};

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */

#endif /* FORCECONTROLLER_H_ */
