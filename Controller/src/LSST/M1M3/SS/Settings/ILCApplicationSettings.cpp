/*
 * ILCTimingSettings.cpp
 *
 *  Created on: Sep 29, 2017
 *      Author: ccontaxis
 */

#include <ILCApplicationSettings.h>
#include <pugixml.hpp>
#include <boost/lexical_cast.hpp>

using namespace pugi;

namespace LSST {
namespace M1M3 {
namespace SS {

void ILCApplicationSettings::load(const std::string &filename) {
	xml_document doc;
	xml_parse_result result = doc.load_file(filename.c_str());
	ReportServerID = boost::lexical_cast<uint32_t>(doc.select_node("//ILCApplicationSettings/Timings/ReportServerID").node().child_value());

	ReportServerID = boost::lexical_cast<uint32_t>(doc.select_node("//ILCApplicationSettings/Timings/ReportServerID").node().child_value());
	ReportServerStatus = boost::lexical_cast<uint32_t>(doc.select_node("//ILCApplicationSettings/Timings/ReportServerStatus").node().child_value());
	ChangeILCMode = boost::lexical_cast<uint32_t>(doc.select_node("//ILCApplicationSettings/Timings/ChangeILCMode").node().child_value());
	BroadcastStepMotor = boost::lexical_cast<uint32_t>(doc.select_node("//ILCApplicationSettings/Timings/BroadcastStepMotor").node().child_value());
	UnicastStepMotor = boost::lexical_cast<uint32_t>(doc.select_node("//ILCApplicationSettings/Timings/UnicastStepMotor").node().child_value());
	ElectromechanicalForceAndStatus = boost::lexical_cast<uint32_t>(doc.select_node("//ILCApplicationSettings/Timings/ElectromechanicalForceAndStatus").node().child_value());
	BroadcastFreezeSensorValues = boost::lexical_cast<uint32_t>(doc.select_node("//ILCApplicationSettings/Timings/BroadcastFreezeSensorValues").node().child_value());
	SetBoostValveDCAGains = boost::lexical_cast<uint32_t>(doc.select_node("//ILCApplicationSettings/Timings/SetBoostValveDCAGains").node().child_value());
	ReadBoostValveDCAGains = boost::lexical_cast<uint32_t>(doc.select_node("//ILCApplicationSettings/Timings/ReadBoostValveDCAGains").node().child_value());
	BroadcastForceDemand = boost::lexical_cast<uint32_t>(doc.select_node("//ILCApplicationSettings/Timings/BroadcastForceDemand").node().child_value());
	UnicastSingleAxisForceDemand = boost::lexical_cast<uint32_t>(doc.select_node("//ILCApplicationSettings/Timings/UnicastSingleAxisForceDemand").node().child_value());
	UnicastDualAxisForceDemand = boost::lexical_cast<uint32_t>(doc.select_node("//ILCApplicationSettings/Timings/UnicastDualAxisForceDemand").node().child_value());
	PneumaticForceAndStatus = boost::lexical_cast<uint32_t>(doc.select_node("//ILCApplicationSettings/Timings/PneumaticForceAndStatus").node().child_value());
	SetADCScanRate = boost::lexical_cast<uint32_t>(doc.select_node("//ILCApplicationSettings/Timings/SetADCScanRate").node().child_value());
	SetADCChannelOffsetAndSensitivity = boost::lexical_cast<uint32_t>(doc.select_node("//ILCApplicationSettings/Timings/SetADCChannelOffsetAndSensitivity").node().child_value());
	Reset = boost::lexical_cast<uint32_t>(doc.select_node("//ILCApplicationSettings/Timings/Reset").node().child_value());
	ReadCalibration = boost::lexical_cast<uint32_t>(doc.select_node("//ILCApplicationSettings/Timings/ReadCalibration").node().child_value());
	ReadDCAPressureValues = boost::lexical_cast<uint32_t>(doc.select_node("//ILCApplicationSettings/Timings/ReadDCAPressureValues").node().child_value());
	ReportDCAID = boost::lexical_cast<uint32_t>(doc.select_node("//ILCApplicationSettings/Timings/ReportDCAID").node().child_value());
	ReportDCAStatus = boost::lexical_cast<uint32_t>(doc.select_node("//ILCApplicationSettings/Timings/ReportDCAStatus").node().child_value());
}

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */
