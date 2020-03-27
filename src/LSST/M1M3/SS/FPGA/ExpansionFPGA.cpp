/*
 * ExpansionFPGA.cpp
 *
 *  Created on: Jan 9, 2018
 *      Author: ccontaxis
 */

#include <ExpansionFPGA.h>
#include <ExpansionFPGAApplicationSettings.h>
#include <NiFpga_M1M3Expansion.h>
#include <unistd.h>
#include <Log.h>

namespace LSST {
namespace M1M3 {
namespace SS {

ExpansionFPGA::ExpansionFPGA() {
    Log.Debug("ExpansionFPGA: ExpansionFPGA()");
    this->session = 0;
    this->remaining = 0;
}

int32_t ExpansionFPGA::initialize() {
    Log.Debug("ExpansionFPGA: initialize()");
    if (!this->expansionFPGAApplicationSettings->Enabled) {
        return 0;
    }
    return NiFpga_Initialize();
}

int32_t ExpansionFPGA::open() {
    Log.Debug("ExpansionFPGA: open()");
    if (!this->expansionFPGAApplicationSettings->Enabled) {
        return 0;
    }
    int32_t status =
            NiFpga_Open("/usr/ts_M1M3Support/" NiFpga_M1M3Expansion_Bitfile, NiFpga_M1M3Expansion_Signature,
                        this->expansionFPGAApplicationSettings->Resource.c_str(), 0, &(this->session));
    status = NiFpga_Abort(this->session);
    status = NiFpga_Download(this->session);
    status = NiFpga_Reset(this->session);
    status = NiFpga_Run(this->session, 0);
    usleep(1000000);
    return status;
}

int32_t ExpansionFPGA::close() {
    Log.Debug("ExpansionFPGA: close()");
    if (!this->expansionFPGAApplicationSettings->Enabled) {
        return 0;
    }
    return NiFpga_Close(this->session, 0);
}

int32_t ExpansionFPGA::finalize() {
    Log.Debug("ExpansionFPGA: finalize()");
    if (!this->expansionFPGAApplicationSettings->Enabled) {
        return 0;
    }
    return NiFpga_Finalize();
}

bool ExpansionFPGA::isErrorCode(int32_t status) {
    bool isError = NiFpga_IsError(status);
    if (isError) {
        Log.Error("ExpansionFPGA: Error code %d", status);
    }
    return isError;
}

int32_t ExpansionFPGA::sample() {
    if (!this->expansionFPGAApplicationSettings->Enabled) {
        return 0;
    }
    return NiFpga_WriteBool(this->session, NiFpga_M1M3Expansion_ControlBool_Sample, true);
}

int32_t ExpansionFPGA::readSlot1(float* data) {
    if (!this->expansionFPGAApplicationSettings->Enabled) {
        return 0;
    }
    return NiFpga_ReadArraySgl(this->session, NiFpga_M1M3Expansion_IndicatorArraySgl_Slot1, data, 6);
}

int32_t ExpansionFPGA::readSlot2(uint32_t* data) {
    if (!this->expansionFPGAApplicationSettings->Enabled) {
        return 0;
    }
    return NiFpga_ReadU32(this->session, NiFpga_M1M3Expansion_IndicatorU32_Slot2, data);
}

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */
