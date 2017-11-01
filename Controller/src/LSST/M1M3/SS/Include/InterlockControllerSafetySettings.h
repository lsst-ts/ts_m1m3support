/*
 * InterlockControllerSafetySettings.h
 *
 *  Created on: Nov 1, 2017
 *      Author: ccontaxis
 */

#ifndef INTERLOCKCONTROLLERSAFETYSETTINGS_H_
#define INTERLOCKCONTROLLERSAFETYSETTINGS_H_

struct InterlockControllerSafetySettings {
	bool FaultOnHeartbeatStateOutputMismatch;
	bool FaultOnCriticalFaultStateOutputMismatch;
	bool FaultOnMirrorLoweringRaisingStateOutputMismatch;
	bool FaultOnMirrorParkedStateOutputMismatch;
};

#endif /* INTERLOCKCONTROLLERSAFETYSETTINGS_H_ */
