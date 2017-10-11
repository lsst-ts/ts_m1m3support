/*
 * FPGASignals.h
 *
 *  Created on: Oct 10, 2017
 *      Author: ccontaxis
 */

#ifndef FPGASIGNALS_H_
#define FPGASIGNALS_H_

namespace LSST {
namespace M1M3 {
namespace SS {

/*!
 * An enumeration match FPGA signals to port numbers.
 */
struct FPGASignals {
	enum Type {
		NA = 0,
		Slot1AI0 = 1,
		Slot1AI1 = 2,
		Slot1AI2 = 3,
		Slot1AI3 = 4,
		Slot2AI0 = 5,
		Slot2AI1 = 6,
		Slot2AI2 = 7,
		Slot2AI3 = 8,
		Slot3DIO0 = 9,
		Slot3DIO1 = 10,
		Slot3DIO2 = 11,
		Slot3DIO3 = 12,
		Slot3DIO4 = 13,
		Slot3DIO5 = 14,
		Slot3DIO6 = 15,
		Slot3DIO7 = 16,
		Slot4DIO0 = 17,
		Slot4DIO1 = 18,
		Slot4DIO2 = 19,
		Slot4DIO3 = 20,
		Slot4DIO4 = 21,
		Slot4DIO5 = 22,
		Slot4DIO6 = 23,
		Slot4DIO7 = 24,
		Slot5DI0 = 25,
		Slot5DI1 = 26,
		Slot5DI2 = 27,
		Slot5DI3 = 28,
		Slot5DI4 = 29,
		Slot5DI5 = 30,
		Slot5DI6 = 31,
		Slot5DI7 = 32,
		Slot5DI8 = 33,
		Slot5DI9 = 34,
		Slot5DI10 = 35,
		Slot5DI11 = 36,
		Slot5DI12 = 37,
		Slot5DI13 = 38,
		Slot5DI14 = 39,
		Slot5DI15 = 40,
		Slot5DI16 = 41,
		Slot5DI17 = 42,
		Slot5DI18 = 43,
		Slot5DI19 = 44,
		Slot5DI20 = 45,
		Slot5DI21 = 46,
		Slot5DI22 = 47,
		Slot5DI23 = 48,
		Slot5DI24 = 49,
		Slot5DI25 = 50,
		Slot5DI26 = 51,
		Slot5DI27 = 52,
		Slot5DI28 = 53,
		Slot5DI29 = 54,
		Slot5DI30 = 55,
		Slot5DI31 = 56,
		HeartbeatToSafetyController = 57,             // Slot6/CH0
		CriticalFaultToSafetyController = 58,         // Slot6/CH1
		MirrorLoweringRaisingToSafetyController = 59, // Slot6/CH2
		MirrorParkedToSafetyController = 60,          // Slot6/CH3
		AirSupplyValveControl = 61,                   // Slot6/CH4
		MirrorCellLightControl = 62,                  // Slot6/CH5
		Slot6CH6 = 63,
		Slot6CH7 = 64,
		Slot7CH0 = 65,
		Slot7CH1 = 66,
		Slot7CH2 = 67,
		Slot7CH3 = 68,
		Slot7CH4 = 69,
		Slot7CH5 = 70,
		Slot7CH6 = 71,
		Slot7CH7 = 72,
		Slot8Port1 = 73,
		Slot8Port2 = 74,
		Slot8Port3 = 75,
		Slot8Port4 = 76
	};
};

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */

#endif /* FPGASIGNALS_H_ */
