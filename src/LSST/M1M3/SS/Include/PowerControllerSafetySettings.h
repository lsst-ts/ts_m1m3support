/*
 * PowerControllerSafetySettings.h
 *
 *  Created on: Dec 7, 2017
 *      Author: ccontaxis
 */

#ifndef POWERCONTROLLERSAFETYSETTINGS_H_
#define POWERCONTROLLERSAFETYSETTINGS_H_

struct PowerControllerSafetySettings {
	bool FaultOnPowerNetworkAOutputMismatch;
	bool FaultOnPowerNetworkBOutputMismatch;
	bool FaultOnPowerNetworkCOutputMismatch;
	bool FaultOnPowerNetworkDOutputMismatch;
	bool FaultOnAuxPowerNetworkAOutputMismatch;
	bool FaultOnAuxPowerNetworkBOutputMismatch;
	bool FaultOnAuxPowerNetworkCOutputMismatch;
	bool FaultOnAuxPowerNetworkDOutputMismatch;
};

#endif /* POWERCONTROLLERSAFETYSETTINGS_H_ */
