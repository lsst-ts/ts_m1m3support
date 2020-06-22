/*
 * This file is part of LSST M1M3 support system package.
 *
 * Developed for the LSST Data Management System.
 * This product includes software developed by the LSST Project
 * (https://www.lsst.org).
 * See the COPYRIGHT file at the top-level directory of this distribution
 * for details of code ownership.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#include <ILCApplicationSettings.h>
#include <XMLDocLoad.h>
#include <boost/lexical_cast.hpp>

using namespace pugi;

namespace LSST {
namespace M1M3 {
namespace SS {

void ILCApplicationSettings::load(const std::string &filename) {
    xml_document doc;
    XMLDocLoad(filename.c_str(), doc);
    this->ReportServerID = boost::lexical_cast<uint32_t>(
            doc.select_node("//ILCApplicationSettings/Timings/ReportServerID").node().child_value());
    this->ReportServerStatus = boost::lexical_cast<uint32_t>(
            doc.select_node("//ILCApplicationSettings/Timings/ReportServerStatus").node().child_value());
    this->ChangeILCMode = boost::lexical_cast<uint32_t>(
            doc.select_node("//ILCApplicationSettings/Timings/ChangeILCMode").node().child_value());
    this->BroadcastStepMotor = boost::lexical_cast<uint32_t>(
            doc.select_node("//ILCApplicationSettings/Timings/BroadcastStepMotor").node().child_value());
    this->UnicastStepMotor = boost::lexical_cast<uint32_t>(
            doc.select_node("//ILCApplicationSettings/Timings/UnicastStepMotor").node().child_value());
    this->ElectromechanicalForceAndStatus = boost::lexical_cast<uint32_t>(
            doc.select_node("//ILCApplicationSettings/Timings/ElectromechanicalForceAndStatus")
                    .node()
                    .child_value());
    this->BroadcastFreezeSensorValues = boost::lexical_cast<uint32_t>(
            doc.select_node("//ILCApplicationSettings/Timings/BroadcastFreezeSensorValues")
                    .node()
                    .child_value());
    this->SetBoostValveDCAGains = boost::lexical_cast<uint32_t>(
            doc.select_node("//ILCApplicationSettings/Timings/SetBoostValveDCAGains").node().child_value());
    this->ReadBoostValveDCAGains = boost::lexical_cast<uint32_t>(
            doc.select_node("//ILCApplicationSettings/Timings/ReadBoostValveDCAGains").node().child_value());
    this->BroadcastForceDemand = boost::lexical_cast<uint32_t>(
            doc.select_node("//ILCApplicationSettings/Timings/BroadcastForceDemand").node().child_value());
    this->UnicastSingleAxisForceDemand = boost::lexical_cast<uint32_t>(
            doc.select_node("//ILCApplicationSettings/Timings/UnicastSingleAxisForceDemand")
                    .node()
                    .child_value());
    this->UnicastDualAxisForceDemand = boost::lexical_cast<uint32_t>(
            doc.select_node("//ILCApplicationSettings/Timings/UnicastDualAxisForceDemand")
                    .node()
                    .child_value());
    this->PneumaticForceAndStatus = boost::lexical_cast<uint32_t>(
            doc.select_node("//ILCApplicationSettings/Timings/PneumaticForceAndStatus").node().child_value());
    this->SetADCScanRate = boost::lexical_cast<uint32_t>(
            doc.select_node("//ILCApplicationSettings/Timings/SetADCScanRate").node().child_value());
    this->SetADCChannelOffsetAndSensitivity = boost::lexical_cast<uint32_t>(
            doc.select_node("//ILCApplicationSettings/Timings/SetADCChannelOffsetAndSensitivity")
                    .node()
                    .child_value());
    this->Reset = boost::lexical_cast<uint32_t>(
            doc.select_node("//ILCApplicationSettings/Timings/Reset").node().child_value());
    this->ReadCalibration = boost::lexical_cast<uint32_t>(
            doc.select_node("//ILCApplicationSettings/Timings/ReadCalibration").node().child_value());
    this->ReadDCAPressureValues = boost::lexical_cast<uint32_t>(
            doc.select_node("//ILCApplicationSettings/Timings/ReadDCAPressureValues").node().child_value());
    this->ReportDCAID = boost::lexical_cast<uint32_t>(
            doc.select_node("//ILCApplicationSettings/Timings/ReportDCAID").node().child_value());
    this->ReportDCAStatus = boost::lexical_cast<uint32_t>(
            doc.select_node("//ILCApplicationSettings/Timings/ReportDCAStatus").node().child_value());
    this->ReportDCAPressure = boost::lexical_cast<uint32_t>(
            doc.select_node("//ILCApplicationSettings/Timings/ReportDCAPressure").node().child_value());
    this->ReportLVDT = boost::lexical_cast<uint32_t>(
            doc.select_node("//ILCApplicationSettings/Timings/ReportLVDT").node().child_value());
}

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */
