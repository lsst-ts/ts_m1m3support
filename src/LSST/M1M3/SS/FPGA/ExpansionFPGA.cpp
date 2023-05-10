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

#include <ExpansionFPGA.h>
#include <ExpansionFPGAApplicationSettings.h>
#include <NiFpga_ts_M1M3SupportExpansionFPGA.h>
#include <NiError.h>

#include <thread>
#include <unistd.h>

#include <spdlog/spdlog.h>

namespace LSST {
namespace M1M3 {
namespace SS {

ExpansionFPGA::ExpansionFPGA() {
    SPDLOG_DEBUG("ExpansionFPGA: ExpansionFPGA()");
    _session = 0;
    _remaining = 0;
}

ExpansionFPGA::~ExpansionFPGA() {}

void ExpansionFPGA::initialize() {
    SPDLOG_DEBUG("ExpansionFPGA: initialize()");
    if (!expansionFPGAApplicationSettings->Enabled) {
        return;
    }
    NiThrowError(__PRETTY_FUNCTION__, NiFpga_Initialize());
}

void ExpansionFPGA::open() {
    SPDLOG_DEBUG("ExpansionFPGA: open({})", expansionFPGAApplicationSettings->Resource);
    if (!expansionFPGAApplicationSettings->Enabled) {
        return;
    }
    NiOpen("/var/lib/M1M3support", NiFpga_ts_M1M3SupportExpansionFPGA,
           expansionFPGAApplicationSettings->Resource.c_str(), 0, &(_session));

    NiThrowError(__PRETTY_FUNCTION__, "NiFpga_Abort", NiFpga_Abort(_session));
    NiThrowError(__PRETTY_FUNCTION__, "NiFpga_Download", NiFpga_Download(_session));
    NiThrowError(__PRETTY_FUNCTION__, "NiFpga_Reset", NiFpga_Reset(_session));
    NiThrowError(__PRETTY_FUNCTION__, "NiFpga_Run", NiFpga_Run(_session, 0));
    std::this_thread::sleep_for(std::chrono::seconds(1));
}

void ExpansionFPGA::close() {
    SPDLOG_DEBUG("ExpansionFPGA: close()");
    if (!expansionFPGAApplicationSettings->Enabled) {
        return;
    }
    NiThrowError(__PRETTY_FUNCTION__, NiFpga_Close(_session, 0));
}

void ExpansionFPGA::finalize() {
    SPDLOG_DEBUG("ExpansionFPGA: finalize()");
    if (!expansionFPGAApplicationSettings->Enabled) {
        return;
    }
    NiThrowError(__PRETTY_FUNCTION__, NiFpga_Finalize());
}

void ExpansionFPGA::sample() {
    if (!expansionFPGAApplicationSettings->Enabled) {
        return;
    }
    NiThrowError(__PRETTY_FUNCTION__,
                 NiFpga_WriteBool(_session, NiFpga_ts_M1M3SupportExpansionFPGA_ControlBool_Sample, true));
}

void ExpansionFPGA::readSlot1(float* data) {
    if (!expansionFPGAApplicationSettings->Enabled) {
        return;
    }
    NiThrowError(__PRETTY_FUNCTION__,
                 NiFpga_ReadArraySgl(_session, NiFpga_ts_M1M3SupportExpansionFPGA_IndicatorArraySgl_Slot1,
                                     data, 6));
}

void ExpansionFPGA::readSlot2(uint32_t* data) {
    if (!expansionFPGAApplicationSettings->Enabled) {
        return;
    }
    NiThrowError(__PRETTY_FUNCTION__,
                 NiFpga_ReadU32(_session, NiFpga_ts_M1M3SupportExpansionFPGA_IndicatorU32_Slot2, data));
}

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */
