/*
 * InclinometerSafetySettings.h
 *
 *  Created on: Oct 27, 2017
 *      Author: ccontaxis
 */

#ifndef INCLINOMETERSAFETYSETTINGS_H_
#define INCLINOMETERSAFETYSETTINGS_H_

struct InclinometerSafetySettings {
	bool FaultOnResponseTimeout;
	bool FaultOnInvalidCRC;
	bool FaultOnUnknownAddress;
	bool FaultOnUnknownFunction;
	bool FaultOnInvalidLength;
	bool FaultOnSensorReportsIllegalDataAddress;
	bool FaultOnSensorReportsIllegalFunction;
	bool FaultOnUnknownProblem;
};

#endif /* INCLINOMETERSAFETYSETTINGS_H_ */
