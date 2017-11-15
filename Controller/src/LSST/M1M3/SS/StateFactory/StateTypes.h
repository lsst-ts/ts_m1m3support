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
		OfflineState             = 0x00000000,
		StandbyState             = 0x01000000,
		DisabledState            = 0x02000000,
		EnabledState             = 0x03000000,
		ParkedState              = 0x03000001,
		RaisingState             = 0x03000002,
		ActiveState              = 0x03000003,
		LoweringState            = 0x03000004,
		EngineeringState         = 0x03010000,
		ParkedEngineeringState   = 0x03010001,
		RaisingEngineeringState  = 0x03010002,
		ActiveEngineeringState   = 0x03010003,
		LoweringEngineeringState = 0x03010004,
		FaultState               = 0xFFFFFFFF,
		NoStateTransition        = 0x7FFFFFFF
	};
};

#endif /* STATETYPES_H_ */
