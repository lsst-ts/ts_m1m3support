/*
 * StateTypes.h
 *
 *  Created on: Sep 26, 2017
 *      Author: ccontaxis
 */

#ifndef STATETYPES_H_
#define STATETYPES_H_

#include <DataTypes.h>
#include <ccpp_sal_m1m3.h> // Provides access to enumerations

struct States {
	enum Type {
//		StateName                         SState                                                  DState
		OfflineState                    = (((uint64_t)m1m3::SummaryState_OfflineState)  << 32) | ((uint64_t)m1m3::DetailedState_OfflineState),
		StandbyState                    = (((uint64_t)m1m3::SummaryState_StandbyState)  << 32) | ((uint64_t)m1m3::DetailedState_StandbyState),
		DisabledState                   = (((uint64_t)m1m3::SummaryState_DisabledState) << 32) | ((uint64_t)m1m3::DetailedState_DisabledState),
		EnabledState                    = (((uint64_t)m1m3::SummaryState_EnabledState)  << 32) | ((uint64_t)m1m3::DetailedState_EnabledState),
		FaultState                      = (((uint64_t)m1m3::SummaryState_FaultState)    << 32) | ((uint64_t)m1m3::DetailedState_FaultState),
		ParkedState                     = (((uint64_t)m1m3::SummaryState_EnabledState)  << 32) | ((uint64_t)m1m3::DetailedState_ParkedState),
		RaisingState                    = (((uint64_t)m1m3::SummaryState_EnabledState)  << 32) | ((uint64_t)m1m3::DetailedState_RaisingState),
		ActiveState                     = (((uint64_t)m1m3::SummaryState_EnabledState)  << 32) | ((uint64_t)m1m3::DetailedState_ActiveState),
		LoweringState                   = (((uint64_t)m1m3::SummaryState_EnabledState)  << 32) | ((uint64_t)m1m3::DetailedState_LoweringState),
		EngineeringState                = (((uint64_t)m1m3::SummaryState_EnabledState)  << 32) | ((uint64_t)m1m3::DetailedState_EngineeringState),
		ParkedEngineeringState          = (((uint64_t)m1m3::SummaryState_EnabledState)  << 32) | ((uint64_t)m1m3::DetailedState_ParkedEngineeringState),
		RaisingEngineeringState         = (((uint64_t)m1m3::SummaryState_EnabledState)  << 32) | ((uint64_t)m1m3::DetailedState_RaisingEngineeringState),
		ActiveEngineeringState          = (((uint64_t)m1m3::SummaryState_EnabledState)  << 32) | ((uint64_t)m1m3::DetailedState_ActiveEngineeringState),
		LoweringEngineeringState        = (((uint64_t)m1m3::SummaryState_EnabledState)  << 32) | ((uint64_t)m1m3::DetailedState_LoweringEngineeringState),
		LoweringFaultState              = (((uint64_t)m1m3::SummaryState_FaultState)    << 32) | ((uint64_t)m1m3::DetailedState_LoweringFaultState),
		ProfileHardpointCorrectionState = (((uint64_t)m1m3::SummaryState_EnabledState)  << 32) | ((uint64_t)m1m3::DetailedState_ProfileHardpointCorrectionState),
		NoStateTransition               = 0xFFFFFFFFFFFFFFFF
	};
};

#endif /* STATETYPES_H_ */
