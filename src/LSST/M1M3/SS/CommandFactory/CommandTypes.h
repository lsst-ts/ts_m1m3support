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
        ApplyAberrationForcesByBendingModesCommand = 13,
        ApplyAberrationForcesCommand = 14,
        ClearAberrationForcesCommand = 15,
        ApplyActiveOpticForcesByBendingModesCommand = 16,
        ApplyActiveOpticForcesCommand = 17,
        ClearActiveOpticForcesCommand = 18,
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
        DisableHardpointCorrectionsCommand = 38,
        RunMirrorForceProfileCommand = 39,
        AbortProfileCommand = 40,
        ApplyOffsetForcesByMirrorForceCommand = 41,
        UpdatePIDCommand = 42,
        ResetPIDCommand = 43,
        ProgramILCCommand = 44,
        ModbusTransmitCommand = 45
    };
};

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */

#endif /* COMMANDTYPES_H_ */
