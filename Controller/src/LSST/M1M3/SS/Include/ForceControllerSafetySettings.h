/*
 * ForceControllerSafetySettings.h
 *
 *  Created on: Nov 15, 2017
 *      Author: ccontaxis
 */

#ifndef FORCECONTROLLERSAFETYSETTINGS_H_
#define FORCECONTROLLERSAFETYSETTINGS_H_

struct ForceControllerSafetySettings {
	bool FaultOnSafetyLimit;
	bool FaultOnXMomentLimit;
	bool FaultOnYMomentLimit;
	bool FaultOnZMomentLimit;
	bool FaultOnNearNeighborCheck;
	bool FaultOnMagnitudeLimit;
	bool FaultOnFarNeighborCheck;
	bool FaultOnElevationForceClipping;
	bool FaultOnAzimuthForceClipping;
	bool FaultOnTemperatureForceClipping;
	bool FaultOnHardpointOffloadForceClipping;
	bool FaultOnDynamicForceClipping;
	bool FaultOnAOSNetForceCheck;
	bool FaultOnAOSForceClipping;
	bool FaultOnStaticForceClipping;
	bool FaultOnAberrationNetForceCheck;
	bool FaultOnAberrationForceClipping;
	bool FaultOnOffsetForceClipping;
};

#endif /* FORCECONTROLLERSAFETYSETTINGS_H_ */
