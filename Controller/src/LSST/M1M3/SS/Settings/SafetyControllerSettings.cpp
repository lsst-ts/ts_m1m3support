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
	this->Displacement.FaultOnInvalidResponse = boost::lexical_cast<int32_t>(doc.select_node("//SafetyControllerSettings/DisplacementSettings/FaultOnInvalidResponse").node().child_value()) != 0;
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
	this->Interlock.FaultOnAuxPowerNetworksOff = boost::lexical_cast<int32_t>(doc.select_node("//SafetyControllerSettings/InterlockSettings/FaultOnAuxPowerNetworksOff").node().child_value()) != 0;
	this->Interlock.FaultOnThermalEquipmentOff = boost::lexical_cast<int32_t>(doc.select_node("//SafetyControllerSettings/InterlockSettings/FaultOnThermalEquipmentOff").node().child_value()) != 0;
	this->Interlock.FaultOnAirSupplyOff = boost::lexical_cast<int32_t>(doc.select_node("//SafetyControllerSettings/InterlockSettings/FaultOnAirSupplyOff").node().child_value()) != 0;
	this->Interlock.FaultOnCabinetDoorOpen = boost::lexical_cast<int32_t>(doc.select_node("//SafetyControllerSettings/InterlockSettings/FaultOnCabinetDoorOpen").node().child_value()) != 0;
	this->Interlock.FaultOnTMAMotionStop = boost::lexical_cast<int32_t>(doc.select_node("//SafetyControllerSettings/InterlockSettings/FaultOnTMAMotionStop").node().child_value()) != 0;
	this->Interlock.FaultOnGISHeartbeatLost = boost::lexical_cast<int32_t>(doc.select_node("//SafetyControllerSettings/InterlockSettings/FaultOnGISHeartbeatLost").node().child_value()) != 0;

	this->ForceController.FaultOnSafetyLimit = boost::lexical_cast<int32_t>(doc.select_node("//SafetyControllerSettings/ForceControllerSettings/FaultOnSafetyLimit").node().child_value()) != 0;
	this->ForceController.FaultOnXMomentLimit = boost::lexical_cast<int32_t>(doc.select_node("//SafetyControllerSettings/ForceControllerSettings/FaultOnXMomentLimit").node().child_value()) != 0;
	this->ForceController.FaultOnYMomentLimit = boost::lexical_cast<int32_t>(doc.select_node("//SafetyControllerSettings/ForceControllerSettings/FaultOnYMomentLimit").node().child_value()) != 0;
	this->ForceController.FaultOnZMomentLimit = boost::lexical_cast<int32_t>(doc.select_node("//SafetyControllerSettings/ForceControllerSettings/FaultOnZMomentLimit").node().child_value()) != 0;
	this->ForceController.FaultOnNearNeighborCheck = boost::lexical_cast<int32_t>(doc.select_node("//SafetyControllerSettings/ForceControllerSettings/FaultOnNearNeighborCheck").node().child_value()) != 0;
	this->ForceController.FaultOnMagnitudeLimit = boost::lexical_cast<int32_t>(doc.select_node("//SafetyControllerSettings/ForceControllerSettings/FaultOnMagnitudeLimit").node().child_value()) != 0;
	this->ForceController.FaultOnFarNeighborCheck = boost::lexical_cast<int32_t>(doc.select_node("//SafetyControllerSettings/ForceControllerSettings/FaultOnFarNeighborCheck").node().child_value()) != 0;
	this->ForceController.FaultOnElevationForceClipping = boost::lexical_cast<int32_t>(doc.select_node("//SafetyControllerSettings/ForceControllerSettings/FaultOnElevationForceClipping").node().child_value()) != 0;
	this->ForceController.FaultOnAzimuthForceClipping = boost::lexical_cast<int32_t>(doc.select_node("//SafetyControllerSettings/ForceControllerSettings/FaultOnAzimuthForceClipping").node().child_value()) != 0;
	this->ForceController.FaultOnThermalForceClipping = boost::lexical_cast<int32_t>(doc.select_node("//SafetyControllerSettings/ForceControllerSettings/FaultOnThermalForceClipping").node().child_value()) != 0;
	this->ForceController.FaultOnBalanceForceClipping = boost::lexical_cast<int32_t>(doc.select_node("//SafetyControllerSettings/ForceControllerSettings/FaultOnBalanceForceClipping").node().child_value()) != 0;
	this->ForceController.FaultOnAccelerationForceClipping = boost::lexical_cast<int32_t>(doc.select_node("//SafetyControllerSettings/ForceControllerSettings/FaultOnAccelerationForceClipping").node().child_value()) != 0;
	this->ForceController.FaultOnActiveOpticNetForceCheck = boost::lexical_cast<int32_t>(doc.select_node("//SafetyControllerSettings/ForceControllerSettings/FaultOnActiveOpticNetForceCheck").node().child_value()) != 0;
	this->ForceController.FaultOnActiveOpticForceClipping = boost::lexical_cast<int32_t>(doc.select_node("//SafetyControllerSettings/ForceControllerSettings/FaultOnActiveOpticForceClipping").node().child_value()) != 0;
	this->ForceController.FaultOnStaticForceClipping = boost::lexical_cast<int32_t>(doc.select_node("//SafetyControllerSettings/ForceControllerSettings/FaultOnStaticForceClipping").node().child_value()) != 0;
	this->ForceController.FaultOnAberrationNetForceCheck = boost::lexical_cast<int32_t>(doc.select_node("//SafetyControllerSettings/ForceControllerSettings/FaultOnAberrationNetForceCheck").node().child_value()) != 0;
	this->ForceController.FaultOnAberrationForceClipping = boost::lexical_cast<int32_t>(doc.select_node("//SafetyControllerSettings/ForceControllerSettings/FaultOnAberrationForceClipping").node().child_value()) != 0;
	this->ForceController.FaultOnOffsetForceClipping = boost::lexical_cast<int32_t>(doc.select_node("//SafetyControllerSettings/ForceControllerSettings/FaultOnOffsetForceClipping").node().child_value()) != 0;
	this->ForceController.FaultOnVelocityForceClipping = boost::lexical_cast<int32_t>(doc.select_node("//SafetyControllerSettings/ForceControllerSettings/FaultOnVelocityForceClipping").node().child_value()) != 0;
	this->ForceController.FaultOnForceClipping = boost::lexical_cast<int32_t>(doc.select_node("//SafetyControllerSettings/ForceControllerSettings/FaultOnForceClipping").node().child_value()) != 0;

	this->CellLights.FaultOnOutputMismatch = boost::lexical_cast<int32_t>(doc.select_node("//SafetyControllerSettings/CellLightSettings/FaultOnOutputMismatch").node().child_value()) != 0;
	this->CellLights.FaultOnSensorMismatch = boost::lexical_cast<int32_t>(doc.select_node("//SafetyControllerSettings/CellLightSettings/FaultOnSensorMismatch").node().child_value()) != 0;

	this->PowerController.FaultOnPowerNetworkAOutputMismatch = boost::lexical_cast<int32_t>(doc.select_node("//SafetyControllerSettings/PowerControllerSettings/FaultOnPowerNetworkAOutputMismatch").node().child_value()) != 0;
	this->PowerController.FaultOnPowerNetworkBOutputMismatch = boost::lexical_cast<int32_t>(doc.select_node("//SafetyControllerSettings/PowerControllerSettings/FaultOnPowerNetworkBOutputMismatch").node().child_value()) != 0;
	this->PowerController.FaultOnPowerNetworkCOutputMismatch = boost::lexical_cast<int32_t>(doc.select_node("//SafetyControllerSettings/PowerControllerSettings/FaultOnPowerNetworkCOutputMismatch").node().child_value()) != 0;
	this->PowerController.FaultOnPowerNetworkDOutputMismatch = boost::lexical_cast<int32_t>(doc.select_node("//SafetyControllerSettings/PowerControllerSettings/FaultOnPowerNetworkDOutputMismatch").node().child_value()) != 0;
	this->PowerController.FaultOnAuxPowerNetworkAOutputMismatch = boost::lexical_cast<int32_t>(doc.select_node("//SafetyControllerSettings/PowerControllerSettings/FaultOnAuxPowerNetworkAOutputMismatch").node().child_value()) != 0;
	this->PowerController.FaultOnAuxPowerNetworkBOutputMismatch = boost::lexical_cast<int32_t>(doc.select_node("//SafetyControllerSettings/PowerControllerSettings/FaultOnAuxPowerNetworkBOutputMismatch").node().child_value()) != 0;
	this->PowerController.FaultOnAuxPowerNetworkCOutputMismatch = boost::lexical_cast<int32_t>(doc.select_node("//SafetyControllerSettings/PowerControllerSettings/FaultOnAuxPowerNetworkCOutputMismatch").node().child_value()) != 0;
	this->PowerController.FaultOnAuxPowerNetworkDOutputMismatch = boost::lexical_cast<int32_t>(doc.select_node("//SafetyControllerSettings/PowerControllerSettings/FaultOnAuxPowerNetworkDOutputMismatch").node().child_value()) != 0;

	this->RaiseOperation.FaultOnTimeout = boost::lexical_cast<int32_t>(doc.select_node("//SafetyControllerSettings/RaiseOperationSettings/FaultOnTimeout").node().child_value()) != 0;

	this->LowerOperation.FaultOnTimeout = boost::lexical_cast<int32_t>(doc.select_node("//SafetyControllerSettings/LowerOperationSettings/FaultOnTimeout").node().child_value()) != 0;

	this->ILC.FaultOnCommunicationTimeout = boost::lexical_cast<int32_t>(doc.select_node("//SafetyControllerSettings/ILCSettings/FaultOnCommunicationTimeout").node().child_value()) != 0;
	this->ILC.CommunicationTimeoutCountThreshold = boost::lexical_cast<int32_t>(doc.select_node("//SafetyControllerSettings/ILCSettings/CommunicationTimeoutCountThreshold").node().child_value());
	this->ILC.CommunicationTimeoutPeriod = boost::lexical_cast<int32_t>(doc.select_node("//SafetyControllerSettings/ILCSettings/CommunicationTimeoutPeriod").node().child_value());
	this->ILC.FaultOnForceActuatorFollowingError = boost::lexical_cast<int32_t>(doc.select_node("//SafetyControllerSettings/ILCSettings/FaultOnForceActuatorFollowingError").node().child_value()) != 0;
	this->ILC.ForceActuatorFollowingErrorCountThreshold = boost::lexical_cast<int32_t>(doc.select_node("//SafetyControllerSettings/ILCSettings/ForceActuatorFollowingErrorCountThreshold").node().child_value());
	this->ILC.ForceActuatorFollowingErrorPeriod = boost::lexical_cast<int32_t>(doc.select_node("//SafetyControllerSettings/ILCSettings/ForceActuatorFollowingErrorPeriod").node().child_value());
	this->ILC.FaultOnHardpointActuatorLoadCellError = boost::lexical_cast<int32_t>(doc.select_node("//SafetyControllerSettings/ILCSettings/FaultOnHardpointActuatorLoadCellError").node().child_value()) != 0;
	this->ILC.FaultOnHardpointActuatorMeasuredForce = boost::lexical_cast<int32_t>(doc.select_node("//SafetyControllerSettings/ILCSettings/FaultOnHardpointActuatorMeasuredForce").node().child_value()) != 0;
	this->ILC.HardpointActuatorMeasuredForceCountThreshold = boost::lexical_cast<int32_t>(doc.select_node("//SafetyControllerSettings/ILCSettings/HardpointActuatorMeasuredForceCountThreshold").node().child_value());
	this->ILC.HardpointActuatorMeasuredForcePeriod = boost::lexical_cast<int32_t>(doc.select_node("//SafetyControllerSettings/ILCSettings/HardpointActuatorMeasuredForcePeriod").node().child_value());
	this->ILC.FaultOnAirPressure = boost::lexical_cast<int32_t>(doc.select_node("//SafetyControllerSettings/ILCSettings/FaultOnAirPressure").node().child_value()) != 0;
	this->ILC.AirPressureCountThreshold = boost::lexical_cast<int32_t>(doc.select_node("//SafetyControllerSettings/ILCSettings/AirPressureCountThreshold").node().child_value());
	this->ILC.AirPressurePeriod = boost::lexical_cast<int32_t>(doc.select_node("//SafetyControllerSettings/ILCSettings/AirPressurePeriod").node().child_value());
}

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */
