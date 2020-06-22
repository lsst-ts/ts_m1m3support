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
    bool FaultOnAuxPowerNetworksOff;
    bool FaultOnThermalEquipmentOff;
    bool FaultOnAirSupplyOff;
    bool FaultOnCabinetDoorOpen;
    bool FaultOnTMAMotionStop;
    bool FaultOnGISHeartbeatLost;
};

#endif /* INTERLOCKCONTROLLERSAFETYSETTINGS_H_ */
