/*
 * This file is part of LSST M1M3 support system package.
 *
 * Developed for the Vera C. Rubin Telescope and Site System.
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

#ifndef ILCAPPLICATIONSETTINGS_H_
#define ILCAPPLICATIONSETTINGS_H_

#include <yaml-cpp/yaml.h>

#include <cRIO/Singleton.h>

#include <cRIO/DataTypes.h>

namespace LSST {
namespace M1M3 {
namespace SS {

struct ILCApplicationSettings : public cRIO::Singleton<ILCApplicationSettings> {
    ILCApplicationSettings(token);

    void load(YAML::Node doc);

    uint32_t FPGAConfigTimeout;
    uint32_t FPGARealtimeLoopTimeout;

    uint32_t ReportServerID;
    uint32_t ReportServerStatus;
    uint32_t ChangeILCMode;
    uint32_t BroadcastStepMotor;
    uint32_t UnicastStepMotor;
    uint32_t ElectromechanicalForceAndStatus;
    uint32_t BroadcastFreezeSensorValues;
    uint32_t SetBoostValveDCAGains;
    uint32_t ReadBoostValveDCAGains;
    uint32_t BroadcastForceDemand;
    uint32_t UnicastSingleAxisForceDemand;
    uint32_t UnicastDualAxisForceDemand;
    uint32_t PneumaticForceAndStatus;
    uint32_t SetADCScanRate;
    uint32_t SetADCChannelOffsetAndSensitivity;
    uint32_t Reset;
    uint32_t ReadCalibration;
    uint32_t ReadDCAPressureValues;
    uint32_t ReportDCAID;
    uint32_t ReportDCAStatus;
    uint32_t ReportDCAPressure;
    uint32_t ReportLVDT;
};

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */

#endif /* ILCAPPLICATIONSETTINGS_H_ */
