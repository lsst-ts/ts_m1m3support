/*
 * StateTypes.h
 *
 *  Created on: Sep 26, 2017
 *      Author: ccontaxis
 */

#ifndef STATETYPES_H_
#define STATETYPES_H_

struct States {
	enum Type {
//		StateName                  SState    DState
		OfflineState             = 0x0000000000000000,
		StandbyState             = 0x0000000100000001,
		DisabledState            = 0x0000000200000002,
		EnabledState             = 0x0000000300000003,
		ParkedState              = 0x0000000300000005,
		RaisingState             = 0x0000000300000006,
		ActiveState              = 0x0000000300000007,
		LoweringState            = 0x0000000300000008,
		EngineeringState         = 0x0000000300000009,
		ParkedEngineeringState   = 0x000000030000000A,
		RaisingEngineeringState  = 0x000000030000000B,
		ActiveEngineeringState   = 0x000000030000000C,
		LoweringEngineeringState = 0x000000030000000D,
		FaultState               = 0x0000000400000004,
		LoweringFaultState       = 0x000000040000000E,
		NoStateTransition        = 0xFFFFFFFFFFFFFFFF
	};
};

#endif /* STATETYPES_H_ */
