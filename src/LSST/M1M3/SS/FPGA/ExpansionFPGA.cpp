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

#include <ExpansionFPGA.h>
#include <ExpansionFPGAApplicationSettings.h>
#include <NiFpga_ts_M1M3SupportExpansionFPGA.h>
#include <NiStatus.h>
#include <unistd.h>
#include <spdlog/spdlog.h>

namespace LSST {
namespace M1M3 {
namespace SS {

ExpansionFPGA::ExpansionFPGA() {
    spdlog::debug("ExpansionFPGA: ExpansionFPGA()");
    this->session = 0;
    this->remaining = 0;
}

int32_t ExpansionFPGA::initialize() {
    spdlog::debug("ExpansionFPGA: initialize()");
    if (!this->expansionFPGAApplicationSettings->Enabled) {
        return 0;
    }
    return NiFpga_Initialize();
}

int32_t ExpansionFPGA::open() {
    spdlog::debug("ExpansionFPGA: open({})", expansionFPGAApplicationSettings->Resource);
    if (!this->expansionFPGAApplicationSettings->Enabled) {
        return 0;
    }
    int32_t status =
            NiFpga_Open("/home/admin/Bitfiles/" NiFpga_ts_M1M3SupportExpansionFPGA_Bitfile,
                        NiFpga_ts_M1M3SupportExpansionFPGA_Signature,
                        this->expansionFPGAApplicationSettings->Resource.c_str(), 0, &(this->session));
    if (status) {
        return NiReportError(__PRETTY_FUNCTION__, status);
    }

    status = NiFpga_Abort(this->session);
    status = NiFpga_Download(this->session);
    status = NiFpga_Reset(this->session);
    status = NiFpga_Run(this->session, 0);
    usleep(1000000);
    return NiReportError(__PRETTY_FUNCTION__, status);
}

int32_t ExpansionFPGA::close() {
    spdlog::debug("ExpansionFPGA: close()");
    if (!this->expansionFPGAApplicationSettings->Enabled) {
        return 0;
    }
    return NiReportError(__PRETTY_FUNCTION__, NiFpga_Close(this->session, 0));
}

int32_t ExpansionFPGA::finalize() {
    spdlog::debug("ExpansionFPGA: finalize()");
    if (!this->expansionFPGAApplicationSettings->Enabled) {
        return 0;
    }
    return NiReportError(__PRETTY_FUNCTION__, NiFpga_Finalize());
}

bool ExpansionFPGA::isErrorCode(int32_t status) {
    bool isError = NiFpga_IsError(status);
    if (isError) {
        spdlog::error("ExpansionFPGA: Error code {:d}", status);
    }
    return isError;
}

int32_t ExpansionFPGA::sample() {
    if (!this->expansionFPGAApplicationSettings->Enabled) {
        return 0;
    }
    return NiReportError(
            __PRETTY_FUNCTION__,
            NiFpga_WriteBool(this->session, NiFpga_ts_M1M3SupportExpansionFPGA_ControlBool_Sample, true));
}

int32_t ExpansionFPGA::readSlot1(float* data) {
    if (!this->expansionFPGAApplicationSettings->Enabled) {
        return 0;
    }
    return NiReportError(
            __PRETTY_FUNCTION__,
            NiFpga_ReadArraySgl(this->session, NiFpga_ts_M1M3SupportExpansionFPGA_IndicatorArraySgl_Slot1,
                                data, 6));
}

int32_t ExpansionFPGA::readSlot2(uint32_t* data) {
    if (!this->expansionFPGAApplicationSettings->Enabled) {
        return 0;
    }
    return NiReportError(
            __PRETTY_FUNCTION__,
            NiFpga_ReadU32(this->session, NiFpga_ts_M1M3SupportExpansionFPGA_IndicatorU32_Slot2, data));
}

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */
