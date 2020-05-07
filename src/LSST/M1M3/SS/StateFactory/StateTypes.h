/*
 * StateTypes.h
 *
 *  Created on: Sep 26, 2017
 *      Author: ccontaxis
 */

#ifndef STATETYPES_H_
#define STATETYPES_H_

#include <DataTypes.h>
#include <ccpp_sal_MTM1M3.h> // Provides access to enumerations

struct States {
	enum Type {
//		StateName                         SState                                                  DState
		OfflineState                    = (((uint64_t)MTM1M3::MTM1M3_shared_SummaryStates_OfflineState)  << 32) | ((uint64_t)MTM1M3::MTM1M3_shared_DetailedStates_OfflineState),
		StandbyState                    = (((uint64_t)MTM1M3::MTM1M3_shared_SummaryStates_StandbyState)  << 32) | ((uint64_t)MTM1M3::MTM1M3_shared_DetailedStates_StandbyState),
		DisabledState                   = (((uint64_t)MTM1M3::MTM1M3_shared_SummaryStates_DisabledState) << 32) | ((uint64_t)MTM1M3::MTM1M3_shared_DetailedStates_DisabledState),
		EnabledState                    = (((uint64_t)MTM1M3::MTM1M3_shared_SummaryStates_EnabledState)  << 32) | ((uint64_t)MTM1M3::MTM1M3_shared_DetailedStates_EnabledState),
		FaultState                      = (((uint64_t)MTM1M3::MTM1M3_shared_SummaryStates_FaultState)    << 32) | ((uint64_t)MTM1M3::MTM1M3_shared_DetailedStates_FaultState),
		ParkedState                     = (((uint64_t)MTM1M3::MTM1M3_shared_SummaryStates_EnabledState)  << 32) | ((uint64_t)MTM1M3::MTM1M3_shared_DetailedStates_ParkedState),
		RaisingState                    = (((uint64_t)MTM1M3::MTM1M3_shared_SummaryStates_EnabledState)  << 32) | ((uint64_t)MTM1M3::MTM1M3_shared_DetailedStates_RaisingState),
		ActiveState                     = (((uint64_t)MTM1M3::MTM1M3_shared_SummaryStates_EnabledState)  << 32) | ((uint64_t)MTM1M3::MTM1M3_shared_DetailedStates_ActiveState),
		LoweringState                   = (((uint64_t)MTM1M3::MTM1M3_shared_SummaryStates_EnabledState)  << 32) | ((uint64_t)MTM1M3::MTM1M3_shared_DetailedStates_LoweringState),
		EngineeringState                = (((uint64_t)MTM1M3::MTM1M3_shared_SummaryStates_EnabledState)  << 32) | ((uint64_t)MTM1M3::MTM1M3_shared_DetailedStates_EngineeringState),
		ParkedEngineeringState          = (((uint64_t)MTM1M3::MTM1M3_shared_SummaryStates_EnabledState)  << 32) | ((uint64_t)MTM1M3::MTM1M3_shared_DetailedStates_ParkedEngineeringState),
		RaisingEngineeringState         = (((uint64_t)MTM1M3::MTM1M3_shared_SummaryStates_EnabledState)  << 32) | ((uint64_t)MTM1M3::MTM1M3_shared_DetailedStates_RaisingEngineeringState),
		ActiveEngineeringState          = (((uint64_t)MTM1M3::MTM1M3_shared_SummaryStates_EnabledState)  << 32) | ((uint64_t)MTM1M3::MTM1M3_shared_DetailedStates_ActiveEngineeringState),
		LoweringEngineeringState        = (((uint64_t)MTM1M3::MTM1M3_shared_SummaryStates_EnabledState)  << 32) | ((uint64_t)MTM1M3::MTM1M3_shared_DetailedStates_LoweringEngineeringState),
		LoweringFaultState              = (((uint64_t)MTM1M3::MTM1M3_shared_SummaryStates_FaultState)    << 32) | ((uint64_t)MTM1M3::MTM1M3_shared_DetailedStates_LoweringFaultState),
		ProfileHardpointCorrectionState = (((uint64_t)MTM1M3::MTM1M3_shared_SummaryStates_EnabledState)  << 32) | ((uint64_t)MTM1M3::MTM1M3_shared_DetailedStates_ProfileHardpointCorrectionState),
		NoStateTransition               = 0xFFFFFFFFFFFFFFFF
	};
};

#endif /* STATETYPES_H_ */
