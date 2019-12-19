/*
 * DisplacementSafetySettings.h
 *
 *  Created on: Oct 27, 2017
 *      Author: ccontaxis
 */

#ifndef DISPLACEMENTSAFETYSETTINGS_H_
#define DISPLACEMENTSAFETYSETTINGS_H_

struct DisplacementSafetySettings {
    bool FaultOnSensorReportsInvalidCommand;
    bool FaultOnSensorReportsCommunicationTimeoutError;
    bool FaultOnSensorReportsDataLengthError;
    bool FaultOnSensorReportsNumberOfParametersError;
    bool FaultOnSensorReportsParameterError;
    bool FaultOnSensorReportsCommunicationError;
    bool FaultOnSensorReportsIDNumberError;
    bool FaultOnSensorReportsExpansionLineError;
    bool FaultOnSensorReportsWriteControlError;
    bool FaultOnResponseTimeoutError;
    bool FaultOnInvalidLength;
    bool FaultOnInvalidResponse;
    bool FaultOnUnknownCommand;
    bool FaultOnUnknownProblem;
};

#endif /* DISPLACEMENTSAFETYSETTINGS_H_ */
