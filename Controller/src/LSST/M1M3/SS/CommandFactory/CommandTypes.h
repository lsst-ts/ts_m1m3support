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
		ClearAOSCorrectionCommand = 18,
		EnterEngineeringCommand = 19,
		ExitEngineeringCommand = 20,
		TestAirCommand = 21,
		TestHardpointCommand = 22,
		TestForceActuatorCommand = 23,
		MoveHardpointActuatorsCommand = 24,
		EnableHardpointChaseCommand = 25,
		DisableHardpointChaseCommand = 26,
		AbortRaiseM1M3Command = 27,
		TranslateM1M3Command = 28,
		StopHardpointMotionCommand = 29,
		TMAAzimuthSampleCommand = 30,
		TMAElevationSampleCommand = 31,
		PositionM1M3Command = 32,
		TurnLightsOnCommand = 33,
		TurnLightsOffCommand = 34,
		TurnPowerOnCommand = 35,
		TurnPowerOffCommand = 36,
		EnableHardpointCorrectionsCommand = 37,
		DisableHardpointCorrectionsCommand = 38
	};
};

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */

#endif /* COMMANDTYPES_H_ */
