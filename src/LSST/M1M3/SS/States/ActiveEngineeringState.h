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

#ifndef ACTIVEENGINEERINGSTATE_H_
#define ACTIVEENGINEERINGSTATE_H_

#include <EngineeringState.h>

namespace LSST {
namespace M1M3 {
namespace SS {

class ActiveEngineeringState : public EngineeringState {
public:
    ActiveEngineeringState(M1M3SSPublisher* publisher);

    States::Type update(UpdateCommand* command, Model* model);
    States::Type lowerM1M3(LowerM1M3Command* command, Model* model);
    States::Type exitEngineering(ExitEngineeringCommand* command, Model* model);
    States::Type applyAberrationForcesByBendingModes(ApplyAberrationForcesByBendingModesCommand* command,
                                                     Model* model);
    States::Type applyAberrationForces(ApplyAberrationForcesCommand* command, Model* model);
    States::Type clearAberrationForces(ClearAberrationForcesCommand* command, Model* model);
    States::Type applyActiveOpticForcesByBendingModes(ApplyActiveOpticForcesByBendingModesCommand* command,
                                                      Model* model);
    States::Type applyActiveOpticForces(ApplyActiveOpticForcesCommand* command, Model* model);
    States::Type clearActiveOpticForces(ClearActiveOpticForcesCommand* command, Model* model);
    States::Type translateM1M3(TranslateM1M3Command* command, Model* model);
    States::Type positionM1M3(PositionM1M3Command* command, Model* model);
    States::Type enableHardpointCorrections(EnableHardpointCorrectionsCommand* command, Model* model);
    States::Type disableHardpointCorrections(DisableHardpointCorrectionsCommand* command, Model* model);
    States::Type runMirrorForceProfile(RunMirrorForceProfileCommand* command, Model* model);
    States::Type updatePID(UpdatePIDCommand* command, Model* model);
    States::Type resetPID(ResetPIDCommand* command, Model* model);
};

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */

#endif /* ACTIVEENGINEERINGSTATE_H_ */
