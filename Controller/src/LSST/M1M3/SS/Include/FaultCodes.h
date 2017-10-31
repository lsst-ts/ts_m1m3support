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
		AirControllerCommandOutputMismatch                 = 0x000017D500000001,
		AirControllerCommandSensorMismatch                 = 0x000017D500000002,
		DisplacementSensorReportsInvalidCommand            = 0x000017D600000001,
		DisplacementSensorReportsCommunicationTimeoutError = 0x000017D600000002,
		DisplacementSensorReportsDataLengthError           = 0x000017D600000003,
		DisplacementSensorReportsNumberOfParametersError   = 0x000017D600000004,
		DisplacementSensorReportsParameterError            = 0x000017D600000005,
		DisplacementSensorReportsCommunicationError        = 0x000017D600000006,
		DisplacementSensorReportsIDNumberError             = 0x000017D600000007,
		DisplacementSensorReportsExpansionLineError        = 0x000017D600000009,
		DisplacementSensorReportsWriteControlError         = 0x000017D60000000A,
		DisplacementResponseTimeoutError                   = 0x000017D60000000B,
		DisplacementInvalidLength                          = 0x000017D60000000C,
		DisplacementUnknownCommand                         = 0x000017D60000000D,
		DisplacementUnknownProblem                         = 0x000017D60000000E,
		InclinometerResponseTimeout                        = 0x000017D700000001,
		InclinometerInvalidCRC                             = 0x000017D700000002,
		InclinometerUnknownAddress                         = 0x000017D700000003,
		InclinometerUnknownFunction                        = 0x000017D700000004,
		InclinometerInvalidLength                          = 0x000017D700000005,
		InclinometerSensorReportsIllegalDataAddress        = 0x000017D700000006,
		InclinometerSensorReportsIllegalFunction           = 0x000017D700000007,
		InclinometerUnknownProblem                         = 0x000017D700000008,
		UnknownFault                                       = 0x0000180600000000
	};
};

#endif /* FAULTCODES_H_ */
