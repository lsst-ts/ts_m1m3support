/*
 * SafetyControllerSettings.cpp
 *
 *  Created on: Oct 26, 2017
 *      Author: ccontaxis
 */

#include <SafetyControllerSettings.h>
#include <boost/lexical_cast.hpp>
#include <pugixml.hpp>

namespace LSST {
namespace M1M3 {
namespace SS {

void SafetyControllerSettings::load(const std::string &filename) {
	pugi::xml_document doc;
	doc.load_file(filename.c_str());

	this->AirController.FaultOnCommandOutputMismatch = boost::lexical_cast<int32_t>(doc.select_node("//SafetyControllerSettings/AirControllerSettings/FaultOnCommandOutputMismatch").node().child_value()) != 0;
	this->AirController.FaultOnCommandSensorMismatch = boost::lexical_cast<int32_t>(doc.select_node("//SafetyControllerSettings/AirControllerSettings/FaultOnCommandSensorMismatch").node().child_value()) != 0;

	this->Displacement.FaultOnSensorReportsInvalidCommand = boost::lexical_cast<int32_t>(doc.select_node("//SafetyControllerSettings/DisplacementSettings/FaultOnSensorReportsInvalidCommand").node().child_value()) != 0;
	this->Displacement.FaultOnSensorReportsCommunicationTimeoutError = boost::lexical_cast<int32_t>(doc.select_node("//SafetyControllerSettings/DisplacementSettings/FaultOnSensorReportsCommunicationTimeoutError").node().child_value()) != 0;
	this->Displacement.FaultOnSensorReportsDataLengthError = boost::lexical_cast<int32_t>(doc.select_node("//SafetyControllerSettings/DisplacementSettings/FaultOnSensorReportsDataLengthError").node().child_value()) != 0;
	this->Displacement.FaultOnSensorReportsNumberOfParametersError = boost::lexical_cast<int32_t>(doc.select_node("//SafetyControllerSettings/DisplacementSettings/FaultOnSensorReportsNumberOfParametersError").node().child_value()) != 0;
	this->Displacement.FaultOnSensorReportsParameterError = boost::lexical_cast<int32_t>(doc.select_node("//SafetyControllerSettings/DisplacementSettings/FaultOnSensorReportsParameterError").node().child_value()) != 0;
	this->Displacement.FaultOnSensorReportsCommunicationError = boost::lexical_cast<int32_t>(doc.select_node("//SafetyControllerSettings/DisplacementSettings/FaultOnSensorReportsCommunicationError").node().child_value()) != 0;
	this->Displacement.FaultOnSensorReportsIDNumberError = boost::lexical_cast<int32_t>(doc.select_node("//SafetyControllerSettings/DisplacementSettings/FaultOnSensorReportsIDNumberError").node().child_value()) != 0;
	this->Displacement.FaultOnSensorReportsExpansionLineError = boost::lexical_cast<int32_t>(doc.select_node("//SafetyControllerSettings/DisplacementSettings/FaultOnSensorReportsExpansionLineError").node().child_value()) != 0;
	this->Displacement.FaultOnSensorReportsWriteControlError = boost::lexical_cast<int32_t>(doc.select_node("//SafetyControllerSettings/DisplacementSettings/FaultOnSensorReportsWriteControlError").node().child_value()) != 0;
	this->Displacement.FaultOnResponseTimeoutError = boost::lexical_cast<int32_t>(doc.select_node("//SafetyControllerSettings/DisplacementSettings/FaultOnResponseTimeoutError").node().child_value()) != 0;
	this->Displacement.FaultOnInvalidLength = boost::lexical_cast<int32_t>(doc.select_node("//SafetyControllerSettings/DisplacementSettings/FaultOnInvalidLength").node().child_value()) != 0;
	this->Displacement.FaultOnUnknownCommand = boost::lexical_cast<int32_t>(doc.select_node("//SafetyControllerSettings/DisplacementSettings/FaultOnUnknownCommand").node().child_value()) != 0;
	this->Displacement.FaultOnUnknownProblem = boost::lexical_cast<int32_t>(doc.select_node("//SafetyControllerSettings/DisplacementSettings/FaultOnUnknownProblem").node().child_value()) != 0;

	this->Inclinometer.FaultOnResponseTimeout = boost::lexical_cast<int32_t>(doc.select_node("//SafetyControllerSettings/InclinometerSettings/FaultOnResponseTimeout").node().child_value()) != 0;
	this->Inclinometer.FaultOnInvalidCRC = boost::lexical_cast<int32_t>(doc.select_node("//SafetyControllerSettings/InclinometerSettings/FaultOnInvalidCRC").node().child_value()) != 0;
	this->Inclinometer.FaultOnUnknownAddress = boost::lexical_cast<int32_t>(doc.select_node("//SafetyControllerSettings/InclinometerSettings/FaultOnUnknownAddress").node().child_value()) != 0;
	this->Inclinometer.FaultOnUnknownFunction = boost::lexical_cast<int32_t>(doc.select_node("//SafetyControllerSettings/InclinometerSettings/FaultOnUnknownFunction").node().child_value()) != 0;
	this->Inclinometer.FaultOnInvalidLength = boost::lexical_cast<int32_t>(doc.select_node("//SafetyControllerSettings/InclinometerSettings/FaultOnInvalidLength").node().child_value()) != 0;
	this->Inclinometer.FaultOnSensorReportsIllegalDataAddress = boost::lexical_cast<int32_t>(doc.select_node("//SafetyControllerSettings/InclinometerSettings/FaultOnSensorReportsIllegalDataAddress").node().child_value()) != 0;
	this->Inclinometer.FaultOnSensorReportsIllegalFunction = boost::lexical_cast<int32_t>(doc.select_node("//SafetyControllerSettings/InclinometerSettings/FaultOnSensorReportsIllegalFunction").node().child_value()) != 0;
	this->Inclinometer.FaultOnUnknownProblem = boost::lexical_cast<int32_t>(doc.select_node("//SafetyControllerSettings/InclinometerSettings/FaultOnUnknownProblem").node().child_value()) != 0;

	this->Interlock.FaultOnHeartbeatStateOutputMismatch = boost::lexical_cast<int32_t>(doc.select_node("//SafetyControllerSettings/InterlockSettings/FaultOnHeartbeatStateOutputMismatch").node().child_value()) != 0;
	this->Interlock.FaultOnCriticalFaultStateOutputMismatch = boost::lexical_cast<int32_t>(doc.select_node("//SafetyControllerSettings/InterlockSettings/FaultOnCriticalFaultStateOutputMismatch").node().child_value()) != 0;
	this->Interlock.FaultOnMirrorLoweringRaisingStateOutputMismatch = boost::lexical_cast<int32_t>(doc.select_node("//SafetyControllerSettings/InterlockSettings/FaultOnMirrorLoweringRaisingStateOutputMismatch").node().child_value()) != 0;
	this->Interlock.FaultOnMirrorParkedStateOutputMismatch = boost::lexical_cast<int32_t>(doc.select_node("//SafetyControllerSettings/InterlockSettings/FaultOnMirrorParkedStateOutputMismatch").node().child_value()) != 0;
}

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */
