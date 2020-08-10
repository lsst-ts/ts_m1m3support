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

#ifndef STATICSTATEFACTORY_H_
#define STATICSTATEFACTORY_H_

#include <OfflineState.h>
#include <StandbyState.h>
#include <DisabledState.h>
#include <ParkedState.h>
#include <RaisingState.h>
#include <ActiveState.h>
#include <LoweringState.h>
#include <ParkedEngineeringState.h>
#include <RaisingEngineeringState.h>
#include <ActiveEngineeringState.h>
#include <LoweringEngineeringState.h>
#include <FaultState.h>
#include <LoweringFaultState.h>
#include <ProfileHardpointCorrectionState.h>

namespace LSST {
namespace M1M3 {
namespace SS {

class StaticStateFactory {
public:
    StaticStateFactory();

    static StaticStateFactory& get();

    State* create(States::Type state);

private:
    StaticStateFactory& operator=(const StaticStateFactory&) = delete;
    StaticStateFactory(const StaticStateFactory&) = delete;

    OfflineState _offlineState;
    StandbyState _standbyState;
    DisabledState _disabledState;
    ParkedState _parkedState;
    RaisingState _raisingState;
    ActiveState _activeState;
    LoweringState _loweringState;
    ParkedEngineeringState _parkedEngineeringState;
    RaisingEngineeringState _raisingEngineeringState;
    ActiveEngineeringState _activeEngineeringState;
    LoweringEngineeringState _loweringEngineeringState;
    FaultState _faultState;
    LoweringFaultState _loweringFaultState;
    ProfileHardpointCorrectionState _profileHardpointCorrectionState;
};

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */

#endif /* STATICSTATEFACTORY_H_ */
