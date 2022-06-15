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

#ifndef LSST_M1M3_SS_FPGA_IEXPANSIONFPGA_H_
#define LSST_M1M3_SS_FPGA_IEXPANSIONFPGA_H_

#include <ExpansionFPGAApplicationSettings.h>

namespace LSST {
namespace M1M3 {
namespace SS {

/**
 * Abstract interface for communication with expansion FPGA. Expansion FPGA
 * controls various relays and measures voltage on the power buses.
 *
 * The class is parent to two child classes, ExpansionFPGA and
 * SimulatedExpansionFPGA.
 *
 * @see ExpansionFPGA
 * @see SimulatedExpansionFPGA
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

    /**
     * Initialize FPGA.
     *
     * @throw NiError on NI error
     */
    virtual void initialize() = 0;

    /**
     * Load & run FPGA code, setup interrupts.
     *
     * @throw NiError on NI error
     */
    virtual void open() = 0;

    /**
     * Close FPGA, stop FPGA code.
     *
     * @throw NiError on NI error
     */
    virtual void close() = 0;

    /**
     * Should be called after closing FPGA.
     *
     * @throw NiError on NI error
     */
    virtual void finalize() = 0;

    /**
     * Writes sample request. After sample request, measured values can be read
     * from float array and U32 indicators.
     *
     * @throw NiError on NI error
     */
    virtual void sample() = 0;

    /**
     * Read 6 float from float array indicator. Data corresponds to Expansion
     * FPGA slot 1 channels.
     *
     * Data:
     *
     * 1. current meter DC A [V]
     * 2. current meter DC B [V]
     * 3. current meter DC C [V]
     * 4. current meter DC D [V]
     * 5. current meter controllers [V]
     * 6. current meter lights [V]
     *
     * @param data fills values from indicators
     *
     * @throw NiError on NI error
     */
    virtual void readSlot1(float* data) = 0;

    /**
     * Reads 32 bitfield of DIO status. The reported values are:
     *
     * 1. MCU - 1 Circuit breaker auxiliary contact
     * 2. MCU - 2 Circuit breaker auxiliary contact
     * 3. MCU - 3 Circuit breaker auxiliary contact
     * 4. Support cabinet utility outlet Circuit breaker auxiliary contact
     * 5. External equipment Circuit breaker auxiliary contact
     * 6. DC power network A redundancy module status
     * 7. DC power network B redundancy module status
     * 8. DC power network C redundancy module status
     * 9. DC power network D redundancy module status
     * 10. DC power network controllers redundancy module status DC network A power supply 1 status
     * 11. DC network A power supply 2 status
     * 12. DC network B power supply 3 status
     * 13. DC network B power supply 4 status
     * 14. DC network C power supply 5 status
     * 15. DC network C power supply 6 status
     * 16. DC network D power supply 7 status
     * 17. DC network D power supply 8 status
     * 18. DC network controllers power supply 9 status DC network LED lights power supply 11 status
     * 19. DC network external control power supply 12 status
     * 20. DC network laser tracker power supply 13 status
     *
     * @throw NiError on NI error
     */
    virtual void readSlot2(uint32_t* data) = 0;

protected:
    ExpansionFPGAApplicationSettings* expansionFPGAApplicationSettings;

private:
    IExpansionFPGA& operator=(const IExpansionFPGA&) = delete;
    IExpansionFPGA(const IExpansionFPGA&) = delete;
};

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */

#endif /* LSST_M1M3_SS_FPGA_IEXPANSIONFPGA_H_ */
