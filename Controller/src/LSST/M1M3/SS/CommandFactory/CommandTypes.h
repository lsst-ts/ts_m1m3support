/*
 * CommandTypes.h
 *
 *  Created on: Sep 29, 2017
 *      Author: ccontaxis
 */

#ifndef COMMANDTYPES_H_
#define COMMANDTYPES_H_

namespace LSST {
namespace M1M3 {
namespace SS {

/*!
 * An enumeration that represents the types of commands that are available.
 */
struct Commands {
	enum Type {
		BootCommand = 0,
		StartCommand = 1,
		EnableCommand = 2,
		DisableCommand = 3,
		StandbyCommand = 4,
		UpdateCommand = 5,
		ShutdownCommand = 6,
		TurnAirOnCommand = 7,
		TurnAirOffCommand = 8,
		ApplyOffsetForcesCommand = 9,
		ClearOffsetForcesCommand = 10,
		RaiseM1M3Command = 11,
		LowerM1M3Command = 12,
		ApplyAberrationByBendingModesCommand = 13,
		ApplyAberrationByForcesCommand = 14,
		ClearAberrationCommand = 15,
		ApplyAOSCorrectionByBendingModesCommand = 16,
		ApplyAOSCorrectionByForcesCommand = 17,
		ClearAOSCorrectionCommand = 18
	};
};

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */

#endif /* COMMANDTYPES_H_ */
