/*
 * This file is part of LSST M1M3 support system package.
 *
 * Developed for the Vera C. Rubin Telescope and Site System.
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

#ifndef STATETYPES_H_
#define STATETYPES_H_

#include <cRIO/DataTypes.h>
#include <SAL_MTM1M3.h>  // Provides access to enumerations

struct States {
    enum Type {
        //		StateName                         SState DState
        OfflineState = (((uint64_t)MTM1M3::MTM1M3_shared_SummaryStates_OfflineState) << 32) |
                       ((uint64_t)MTM1M3::MTM1M3_shared_DetailedStates_OfflineState),
        StandbyState = (((uint64_t)MTM1M3::MTM1M3_shared_SummaryStates_StandbyState) << 32) |
                       ((uint64_t)MTM1M3::MTM1M3_shared_DetailedStates_StandbyState),
        DisabledState = (((uint64_t)MTM1M3::MTM1M3_shared_SummaryStates_DisabledState) << 32) |
                        ((uint64_t)MTM1M3::MTM1M3_shared_DetailedStates_DisabledState),
        FaultState = (((uint64_t)MTM1M3::MTM1M3_shared_SummaryStates_FaultState) << 32) |
                     ((uint64_t)MTM1M3::MTM1M3_shared_DetailedStates_FaultState),
        ParkedState = (((uint64_t)MTM1M3::MTM1M3_shared_SummaryStates_EnabledState) << 32) |
                      ((uint64_t)MTM1M3::MTM1M3_shared_DetailedStates_ParkedState),
        RaisingState = (((uint64_t)MTM1M3::MTM1M3_shared_SummaryStates_EnabledState) << 32) |
                       ((uint64_t)MTM1M3::MTM1M3_shared_DetailedStates_RaisingState),
        ActiveState = (((uint64_t)MTM1M3::MTM1M3_shared_SummaryStates_EnabledState) << 32) |
                      ((uint64_t)MTM1M3::MTM1M3_shared_DetailedStates_ActiveState),
        LoweringState = (((uint64_t)MTM1M3::MTM1M3_shared_SummaryStates_EnabledState) << 32) |
                        ((uint64_t)MTM1M3::MTM1M3_shared_DetailedStates_LoweringState),
        ParkedEngineeringState = (((uint64_t)MTM1M3::MTM1M3_shared_SummaryStates_EnabledState) << 32) |
                                 ((uint64_t)MTM1M3::MTM1M3_shared_DetailedStates_ParkedEngineeringState),
        RaisingEngineeringState = (((uint64_t)MTM1M3::MTM1M3_shared_SummaryStates_EnabledState) << 32) |
                                  ((uint64_t)MTM1M3::MTM1M3_shared_DetailedStates_RaisingEngineeringState),
        ActiveEngineeringState = (((uint64_t)MTM1M3::MTM1M3_shared_SummaryStates_EnabledState) << 32) |
                                 ((uint64_t)MTM1M3::MTM1M3_shared_DetailedStates_ActiveEngineeringState),
        LoweringEngineeringState = (((uint64_t)MTM1M3::MTM1M3_shared_SummaryStates_EnabledState) << 32) |
                                   ((uint64_t)MTM1M3::MTM1M3_shared_DetailedStates_LoweringEngineeringState),
        LoweringFaultState = (((uint64_t)MTM1M3::MTM1M3_shared_SummaryStates_FaultState) << 32) |
                             ((uint64_t)MTM1M3::MTM1M3_shared_DetailedStates_LoweringFaultState),
        ProfileHardpointCorrectionState =
                (((uint64_t)MTM1M3::MTM1M3_shared_SummaryStates_EnabledState) << 32) |
                ((uint64_t)MTM1M3::MTM1M3_shared_DetailedStates_ProfileHardpointCorrectionState),
        PausedRaisingState = (((uint64_t)MTM1M3::MTM1M3_shared_SummaryStates_EnabledState) << 32) |
                             ((uint64_t)MTM1M3::MTM1M3_shared_DetailedStates_PausedRaisingState),
        PausedRaisingEngineeringState =
                (((uint64_t)MTM1M3::MTM1M3_shared_SummaryStates_EnabledState) << 32) |
                ((uint64_t)MTM1M3::MTM1M3_shared_DetailedStates_PausedRaisingEngineeringState),
        PausedLoweringState = (((uint64_t)MTM1M3::MTM1M3_shared_SummaryStates_EnabledState) << 32) |
                              ((uint64_t)MTM1M3::MTM1M3_shared_DetailedStates_PausedLoweringState),
        PausedLoweringEngineeringState =
                (((uint64_t)MTM1M3::MTM1M3_shared_SummaryStates_EnabledState) << 32) |
                ((uint64_t)MTM1M3::MTM1M3_shared_DetailedStates_PausedLoweringEngineeringState),
        NoStateTransition = 0xFFFFFFFFFFFFFFFF
    };
};

#endif /* STATETYPES_H_ */
