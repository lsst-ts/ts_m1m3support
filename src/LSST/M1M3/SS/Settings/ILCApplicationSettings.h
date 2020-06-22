#ifndef ILCAPPLICATIONSETTINGS_H_
#define ILCAPPLICATIONSETTINGS_H_

#include <DataTypes.h>
#include <string>

namespace LSST {
namespace M1M3 {
namespace SS {

struct ILCApplicationSettings {
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

    void load(const std::string &filename);
};

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */

#endif /* ILCAPPLICATIONSETTINGS_H_ */
