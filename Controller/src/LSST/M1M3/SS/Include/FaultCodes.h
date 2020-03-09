/*
 * FaultCodes.h
 *
 *  Created on: Oct 27, 2017
 *      Author: ccontaxis
 */

#ifndef FAULTCODES_H_
#define FAULTCODES_H_

struct FaultCodes {
	enum Type {
		NoFault                                            = 0x0000000000000000,
		AirControllerCommandOutputMismatch                 = 0x000017D500000001, // 6101
		AirControllerCommandSensorMismatch                 = 0x000017D500000002,
		DisplacementSensorReportsInvalidCommand            = 0x000017D600000001, // 6102
		DisplacementSensorReportsCommunicationTimeoutError = 0x000017D600000002,
		DisplacementSensorReportsDataLengthError           = 0x000017D600000003,
		DisplacementSensorReportsNumberOfParametersError   = 0x000017D600000004,
		DisplacementSensorReportsParameterError            = 0x000017D600000005,
		DisplacementSensorReportsCommunicationError        = 0x000017D600000006,
		DisplacementSensorReportsIDNumberError             = 0x000017D600000007,
		DisplacementSensorReportsExpansionLineError        = 0x000017D600000008,
		DisplacementSensorReportsWriteControlError         = 0x000017D600000009,
		DisplacementResponseTimeoutError                   = 0x000017D60000000A,
		DisplacementInvalidLength                          = 0x000017D60000000B,
		DisplacementUnknownCommand                         = 0x000017D60000000C,
		DisplacementUnknownProblem                         = 0x000017D60000000D,
		DisplacementInvalidResponse                        = 0x000017D60000000E,
		InclinometerResponseTimeout                        = 0x000017D700000001, // 6103
		InclinometerInvalidCRC                             = 0x000017D700000002,
		InclinometerUnknownAddress                         = 0x000017D700000003,
		InclinometerUnknownFunction                        = 0x000017D700000004,
		InclinometerInvalidLength                          = 0x000017D700000005,
		InclinometerSensorReportsIllegalDataAddress        = 0x000017D700000006,
		InclinometerSensorReportsIllegalFunction           = 0x000017D700000007,
		InclinometerUnknownProblem                         = 0x000017D700000008,
		InterlockHeartbeatStateOutputMismatch              = 0x000017D800000001, // 6104
		InterlockCriticalFaultStateOutputMismatch          = 0x000017D800000002,
		InterlockMirrorLoweringRaisingStateOutputMismatch  = 0x000017D800000003,
		InterlockMirrorParkedStateOutputMismatch           = 0x000017D800000004,
		InterlockPowerNetworksOff                          = 0x000017D800000005,
		InterlockThermalEquipmentOff                       = 0x000017D800000006,
		InterlockLaserTrackerOff                           = 0x000017D800000007,
		InterlockAirSupplyOff                              = 0x000017D800000008,
		InterlockGISEarthquake                             = 0x000017D800000009,
		InterlockGISEStop                                  = 0x000017D80000000A,
		InterlockTMAMotionStop                             = 0x000017D80000000B,
		InterlockGISHeartbeatLost                          = 0x000017D80000000C,
		ForceControllerSafetyLimit                         = 0x000017D900000001, // 6105
		ForceControllerXMomentLimit                        = 0x000017D900000002,
		ForceControllerYMomentLimit                        = 0x000017D900000003,
		ForceControllerZMomentLimit                        = 0x000017D900000004,
		ForceControllerNearNeighborCheck                   = 0x000017D900000005,
		ForceControllerMagnitudeLimit                      = 0x000017D900000006,
		ForceControllerFarNeighborCheck                    = 0x000017D900000007,
		ForceControllerElevationForceClipping              = 0x000017D900000008,
		ForceControllerAzimuthForceClipping                = 0x000017D900000009,
		ForceControllerThermalForceClipping                = 0x000017D90000000A,
		ForceControllerBalanceForceClipping                = 0x000017D90000000B,
		ForceControllerAccelerationForceClipping           = 0x000017D90000000C,
		ForceControllerActiveOpticNetForceCheck            = 0x000017D90000000D,
		ForceControllerActiveOpticForceClipping            = 0x000017D90000000E,
		ForceControllerStaticForceClipping                 = 0x000017D90000000F,
		ForceControllerAberrationNetForceCheck             = 0x000017D900000010,
		ForceControllerAberrationForceClipping             = 0x000017D900000011,
		ForceControllerOffsetForceClipping                 = 0x000017D900000012,
		ForceControllerVelocityForceClipping               = 0x000017D900000013,
		ForceControllerForceClipping                       = 0x000017D900000014,
		CellLightOutputMismatch                            = 0x000017DA00000001, // 6106
		CellLightSensorMismatch                            = 0x000017DA00000002,
		PowerControllerPowerNetworkAOutputMismatch         = 0x000017DB00000001, // 6107
		PowerControllerPowerNetworkBOutputMismatch         = 0x000017DB00000002,
		PowerControllerPowerNetworkCOutputMismatch         = 0x000017DB00000003,
		PowerControllerPowerNetworkDOutputMismatch         = 0x000017DB00000004,
		PowerControllerAuxPowerNetworkAOutputMismatch      = 0x000017DB00000005,
		PowerControllerAuxPowerNetworkBOutputMismatch      = 0x000017DB00000006,
		PowerControllerAuxPowerNetworkCOutputMismatch      = 0x000017DB00000007,
		PowerControllerAuxPowerNetworkDOutputMismatch      = 0x000017DB00000008,
		RaiseOperationTimeout                              = 0x000017DC00000001, // 6108
		LowerOperationTimeout                              = 0x000017DD00000001, // 6109
		ILCCommunicationTimeout                            = 0x000017DE00000001, // 6110
		ForceActuatorFollowingError                        = 0x000017DF00000001, // 6111
		HardpointActuator                                  = 0x000017E000000001, // 6112
		HardpointActuatorLoadCellError                     = 0x000017E000000002,
		HardpointActuatorMeasuredForceError                = 0x000017E000000003,
		HardpointActuatorAirPressure                       = 0x000017E000000004,
		UnknownFault                                       = 0x0000180600000001  // 6150
	};
};

#endif /* FAULTCODES_H_ */
