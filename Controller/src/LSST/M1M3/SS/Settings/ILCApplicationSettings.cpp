/*
 * ILCTimingSettings.cpp
 *
 *  Created on: Sep 29, 2017
 *      Author: ccontaxis
 */

#include <ILCApplicationSettings.h>
#include <pugixml.hpp>
#include <boost/lexical_cast.hpp>
#include <Log.h>

using namespace pugi;

namespace LSST {
namespace M1M3 {
namespace SS {

void ILCApplicationSettings::load(const std::string &filename) {
	xml_document doc;
	pugi::xml_parse_result load_file_xml_parse_result = doc.load_file(filename.c_str());
	if (!load_file_xml_parse_result) {
		Log.Fatal("Settings file %s could not be loaded", filename.c_str());
		Log.Fatal("Error description: %s", load_file_xml_parse_result.description());
	}
	this->ReportServerID = boost::lexical_cast<uint32_t>(doc.select_node("//ILCApplicationSettings/Timings/ReportServerID").node().child_value());
	this->ReportServerStatus = boost::lexical_cast<uint32_t>(doc.select_node("//ILCApplicationSettings/Timings/ReportServerStatus").node().child_value());
	this->ChangeILCMode = boost::lexical_cast<uint32_t>(doc.select_node("//ILCApplicationSettings/Timings/ChangeILCMode").node().child_value());
	this->BroadcastStepMotor = boost::lexical_cast<uint32_t>(doc.select_node("//ILCApplicationSettings/Timings/BroadcastStepMotor").node().child_value());
	this->UnicastStepMotor = boost::lexical_cast<uint32_t>(doc.select_node("//ILCApplicationSettings/Timings/UnicastStepMotor").node().child_value());
	this->ElectromechanicalForceAndStatus = boost::lexical_cast<uint32_t>(doc.select_node("//ILCApplicationSettings/Timings/ElectromechanicalForceAndStatus").node().child_value());
	this->BroadcastFreezeSensorValues = boost::lexical_cast<uint32_t>(doc.select_node("//ILCApplicationSettings/Timings/BroadcastFreezeSensorValues").node().child_value());
	this->SetBoostValveDCAGains = boost::lexical_cast<uint32_t>(doc.select_node("//ILCApplicationSettings/Timings/SetBoostValveDCAGains").node().child_value());
	this->ReadBoostValveDCAGains = boost::lexical_cast<uint32_t>(doc.select_node("//ILCApplicationSettings/Timings/ReadBoostValveDCAGains").node().child_value());
	this->BroadcastForceDemand = boost::lexical_cast<uint32_t>(doc.select_node("//ILCApplicationSettings/Timings/BroadcastForceDemand").node().child_value());
	this->UnicastSingleAxisForceDemand = boost::lexical_cast<uint32_t>(doc.select_node("//ILCApplicationSettings/Timings/UnicastSingleAxisForceDemand").node().child_value());
	this->UnicastDualAxisForceDemand = boost::lexical_cast<uint32_t>(doc.select_node("//ILCApplicationSettings/Timings/UnicastDualAxisForceDemand").node().child_value());
	this->PneumaticForceAndStatus = boost::lexical_cast<uint32_t>(doc.select_node("//ILCApplicationSettings/Timings/PneumaticForceAndStatus").node().child_value());
	this->SetADCScanRate = boost::lexical_cast<uint32_t>(doc.select_node("//ILCApplicationSettings/Timings/SetADCScanRate").node().child_value());
	this->SetADCChannelOffsetAndSensitivity = boost::lexical_cast<uint32_t>(doc.select_node("//ILCApplicationSettings/Timings/SetADCChannelOffsetAndSensitivity").node().child_value());
	this->Reset = boost::lexical_cast<uint32_t>(doc.select_node("//ILCApplicationSettings/Timings/Reset").node().child_value());
	this->ReadCalibration = boost::lexical_cast<uint32_t>(doc.select_node("//ILCApplicationSettings/Timings/ReadCalibration").node().child_value());
	this->ReadDCAPressureValues = boost::lexical_cast<uint32_t>(doc.select_node("//ILCApplicationSettings/Timings/ReadDCAPressureValues").node().child_value());
	this->ReportDCAID = boost::lexical_cast<uint32_t>(doc.select_node("//ILCApplicationSettings/Timings/ReportDCAID").node().child_value());
	this->ReportDCAStatus = boost::lexical_cast<uint32_t>(doc.select_node("//ILCApplicationSettings/Timings/ReportDCAStatus").node().child_value());
	this->ReportDCAPressure = boost::lexical_cast<uint32_t>(doc.select_node("//ILCApplicationSettings/Timings/ReportDCAPressure").node().child_value());
	this->ReportLVDT = boost::lexical_cast<uint32_t>(doc.select_node("//ILCApplicationSettings/Timings/ReportLVDT").node().child_value());
}

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */
