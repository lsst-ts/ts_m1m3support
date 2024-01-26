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

#ifndef FAULTCODES_H_
#define FAULTCODES_H_

#include <cstdint>

constexpr int32_t _MASK_AIR = 0x1 << 16;
constexpr int32_t _MASK_DISPLACEMENT = 0x2 << 16;
constexpr int32_t _MASK_INCLINOMETER = 0x3 << 16;
constexpr int32_t _MASK_INTERLOCK = 0x4 << 16;
constexpr int32_t _MASK_FORCE_CONTROLLER = 0x5 << 16;
constexpr int32_t _MASK_CELL_LIGHT = 0x6 << 16;
constexpr int32_t _MASK_POWER_CONTROLLER = 0x7 << 16;
constexpr int32_t _MASK_TIMEOUTS = 0x8 << 16;
constexpr int32_t _MASK_FORCE_ACTUATOR = 0x9 << 16;
constexpr int32_t _MASK_HARDPOINT = 0xA << 16;
constexpr int32_t _MASK_TMA = 0xB << 16;
constexpr int32_t _MASK_USER = 0xC << 16;

/**
 * Fault codes. Implemented as 32bit integer. Send as errorCode in generic
 * event_errorCode.
 */
struct FaultCodes {
    enum Type {
        NoFault = 0x00000000,
        AirControllerCommandOutputMismatch = _MASK_AIR | 0x01,  // 6101
        AirControllerCommandSensorMismatch = _MASK_AIR | 0x02,
        DisplacementSensorReportsInvalidCommand = _MASK_DISPLACEMENT | 0x01,  // 6102
        DisplacementSensorReportsCommunicationTimeoutError = _MASK_DISPLACEMENT | 0x02,
        DisplacementSensorReportsDataLengthError = _MASK_DISPLACEMENT | 0x03,
        DisplacementSensorReportsNumberOfParametersError = _MASK_DISPLACEMENT | 0x04,
        DisplacementSensorReportsParameterError = _MASK_DISPLACEMENT | 0x05,
        DisplacementSensorReportsCommunicationError = _MASK_DISPLACEMENT | 0x05,
        DisplacementSensorReportsIDNumberError = _MASK_DISPLACEMENT | 0x07,
        DisplacementSensorReportsExpansionLineError = _MASK_DISPLACEMENT | 0x08,
        DisplacementSensorReportsWriteControlError = _MASK_DISPLACEMENT | 0x09,
        DisplacementResponseTimeoutError = _MASK_DISPLACEMENT | 0x0A,
        DisplacementInvalidLength = _MASK_DISPLACEMENT | 0x0B,
        DisplacementUnknownCommand = _MASK_DISPLACEMENT | 0x0C,
        DisplacementUnknownProblem = _MASK_DISPLACEMENT | 0x0D,
        DisplacementInvalidResponse = _MASK_DISPLACEMENT | 0x0E,
        InclinometerResponseTimeout = _MASK_INCLINOMETER | 0x01,  // 6103
        InclinometerInvalidCRC = _MASK_INCLINOMETER | 0x02,
        InclinometerUnknownAddress = _MASK_INCLINOMETER | 0x03,
        InclinometerUnknownFunction = _MASK_INCLINOMETER | 0x04,
        InclinometerInvalidLength = _MASK_INCLINOMETER | 0x05,
        InclinometerSensorReportsIllegalDataAddress = _MASK_INCLINOMETER | 0x06,
        InclinometerSensorReportsIllegalFunction = _MASK_INCLINOMETER | 0x07,
        InclinometerUnknownProblem = _MASK_INCLINOMETER | 0x08,
        InterlockHeartbeatStateOutputMismatch = _MASK_INTERLOCK | 0x01,  // 6104
        InterlockCriticalFaultStateOutputMismatch = _MASK_INTERLOCK | 0x02,
        InterlockMirrorLoweringRaisingStateOutputMismatch = _MASK_INTERLOCK | 0x03,
        InterlockMirrorParkedStateOutputMismatch = _MASK_INTERLOCK | 0x04,
        InterlockPowerNetworksOff = _MASK_INTERLOCK | 0x05,
        InterlockThermalEquipmentOff = _MASK_INTERLOCK | 0x06,
        InterlockLaserTrackerOff = _MASK_INTERLOCK | 0x07,
        InterlockAirSupplyOff = _MASK_INTERLOCK | 0x08,
        InterlockGISEarthquake = _MASK_INTERLOCK | 0x09,
        InterlockGISEStop = _MASK_INTERLOCK | 0x0A,
        InterlockTMAMotionStop = _MASK_INTERLOCK | 0x0B,
        InterlockGISHeartbeatLost = _MASK_INTERLOCK | 0x0C,
        ForceControllerSafetyLimit = _MASK_FORCE_CONTROLLER | 0x01,  // 6105
        ForceControllerXMomentLimit = _MASK_FORCE_CONTROLLER | 0x02,
        ForceControllerYMomentLimit = _MASK_FORCE_CONTROLLER | 0x03,
        ForceControllerZMomentLimit = _MASK_FORCE_CONTROLLER | 0x04,
        ForceControllerNearNeighborCheck = _MASK_FORCE_CONTROLLER | 0x05,
        ForceControllerMagnitudeLimit = _MASK_FORCE_CONTROLLER | 0x06,
        ForceControllerFarNeighborCheck = _MASK_FORCE_CONTROLLER | 0x07,
        ForceControllerElevationForceClipping = _MASK_FORCE_CONTROLLER | 0x08,
        ForceControllerAzimuthForceClipping = _MASK_FORCE_CONTROLLER | 0x09,
        ForceControllerThermalForceClipping = _MASK_FORCE_CONTROLLER | 0x0A,
        ForceControllerBalanceForceClipping = _MASK_FORCE_CONTROLLER | 0x0B,
        ForceControllerAccelerationForceClipping = _MASK_FORCE_CONTROLLER | 0x0C,
        ForceControllerActiveOpticNetForceCheck = _MASK_FORCE_CONTROLLER | 0x0D,
        ForceControllerActiveOpticForceClipping = _MASK_FORCE_CONTROLLER | 0x0E,
        ForceControllerStaticForceClipping = _MASK_FORCE_CONTROLLER | 0x0F,
        ForceControllerOffsetForceClipping = _MASK_FORCE_CONTROLLER | 0x10,
        ForceControllerVelocityForceClipping = _MASK_FORCE_CONTROLLER | 0x11,
        ForceControllerForceClipping = _MASK_FORCE_CONTROLLER | 0x12,
        ForceControllerMeasuredXForceLimit = _MASK_FORCE_CONTROLLER | 0x13,
        ForceControllerMeasuredYForceLimit = _MASK_FORCE_CONTROLLER | 0x14,
        ForceControllerMeasuredZForceLimit = _MASK_FORCE_CONTROLLER | 0x15,
        CellLightOutputMismatch = _MASK_CELL_LIGHT | 0x01,  // 6106
        CellLightSensorMismatch = _MASK_CELL_LIGHT | 0x02,
        PowerControllerPowerNetworkAOutputMismatch = _MASK_POWER_CONTROLLER | 0x01,  // 6107
        PowerControllerPowerNetworkBOutputMismatch = _MASK_POWER_CONTROLLER | 0x02,
        PowerControllerPowerNetworkCOutputMismatch = _MASK_POWER_CONTROLLER | 0x03,
        PowerControllerPowerNetworkDOutputMismatch = _MASK_POWER_CONTROLLER | 0x04,
        PowerControllerAuxPowerNetworkAOutputMismatch = _MASK_POWER_CONTROLLER | 0x05,
        PowerControllerAuxPowerNetworkBOutputMismatch = _MASK_POWER_CONTROLLER | 0x06,
        PowerControllerAuxPowerNetworkCOutputMismatch = _MASK_POWER_CONTROLLER | 0x07,
        PowerControllerAuxPowerNetworkDOutputMismatch = _MASK_POWER_CONTROLLER | 0x08,
        RaiseOperationTimeout = _MASK_TIMEOUTS | 0x01,                       // 6108
        LowerOperationTimeout = _MASK_TIMEOUTS | 0x02,                       // 6109
        ILCCommunicationTimeout = _MASK_TIMEOUTS | 0x03,                     // 6110
        ForceActuatorFollowingErrorCounting = _MASK_FORCE_ACTUATOR | 0x01,   // 6111
        ForceActuatorFollowingErrorImmediate = _MASK_FORCE_ACTUATOR | 0x02,  // 0112
        HardpointActuator = _MASK_HARDPOINT | 0x01,                          // 6112
        HardpointActuatorLoadCellError = _MASK_HARDPOINT | 0x02,
        HardpointActuatorMeasuredForceError = _MASK_HARDPOINT | 0x03,
        HardpointHighTension = _MASK_HARDPOINT | 0x04,

        /**
         * Triggered when pressure as measured inside hardpoint breakway
         * mechanism is above of
         * HardpointActuatorSettings/AirPressureWarningHigh and
         * AirPressureWarningLow
         */
        HardpointActuatorAirPressureHigh = _MASK_HARDPOINT | 0x04,

        /**
         * Triggered when pressure as measured inside hardpoint breakway
         * mechanism is below of
         * HardpointActuatorSettings/AirPressureWarningLow
         */
        HardpointActuatorAirPressureLow = _MASK_HARDPOINT | 0x05,

        /**
         * Triggered when pressure as measured inside hardpoint breakway
         * mechanism is outside of
         * HardpointActuatorSettings/AirPressureWarningLow and
         * HardpointActuatorSettings/AirPressureWarningHigh
         */
        HardpointActuatorAirPressureOutside = _MASK_HARDPOINT | 0x06,

        /**
         * Triggered when hardpoint actuator is being commanded to move below
         * low limit switch.
         */
        HardpointActuatorLimitLowError = _MASK_HARDPOINT | 0x07,

        /**
         * Triggered when hardpoint actuator is being commanded to move above
         * low limit switch.
         */
        HardpointActuatorLimitHighError = _MASK_HARDPOINT | 0x08,

        /**
         * Triggered when encoder counts don't match queued steps. Most likely
         * caused by loosed gears between hardpoint motor and actuator shaft.
         */
        HardpointActuatorFollowingError = _MASK_HARDPOINT | 0x09,

        /**
         * Triggered when hardpoint cannot reach given encoder position during
         * FinePositioning.
         */
        HardpointUnstableError = _MASK_HARDPOINT | 0x0A,

        TMAAzimuthTimeout = _MASK_TMA | 0x01,
        TMAElevationTimeout = _MASK_TMA | 0x02,
        TMAInclinometerDeviation = _MASK_TMA | 0x04,

        UserPanic = _MASK_USER | 0x01,
    };
};

#endif /* FAULTCODES_H_ */
