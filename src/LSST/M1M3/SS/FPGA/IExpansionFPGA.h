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

#ifndef LSST_M1M3_SS_FPGA_IEXPANSIONFPGA_H_
#define LSST_M1M3_SS_FPGA_IEXPANSIONFPGA_H_

#include <ExpansionFPGAApplicationSettings.h>

namespace LSST {
namespace M1M3 {
namespace SS {

/**
 * Abstract singleton class for expansion FPGA.
 */
class IExpansionFPGA {
public:
    IExpansionFPGA() { expansionFPGAApplicationSettings = NULL; };
    virtual ~IExpansionFPGA(){};

    static IExpansionFPGA& get();

    void setExpansionFPGAApplicationSettings(
            ExpansionFPGAApplicationSettings* expansionFPGAApplicationSettings) {
        this->expansionFPGAApplicationSettings = expansionFPGAApplicationSettings;
    }

    virtual void initialize() = 0;
    virtual void open() = 0;
    virtual void close() = 0;
    virtual void finalize() = 0;

    virtual void sample() = 0;

    virtual void readSlot1(float* data) = 0;
    virtual void readSlot2(uint32_t* data) = 0;

protected:
    ExpansionFPGAApplicationSettings* expansionFPGAApplicationSettings;
};

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */

#endif /* LSST_M1M3_SS_FPGA_IEXPANSIONFPGA_H_ */
