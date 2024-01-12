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

#ifndef EXPANSIONFPGA_H_
#define EXPANSIONFPGA_H_

#include <string>

#include <IExpansionFPGA.h>

namespace LSST {
namespace M1M3 {
namespace SS {

class ExpansionFPGA : public IExpansionFPGA {
public:
    ExpansionFPGA();
    virtual ~ExpansionFPGA();

    void initialize() override;
    void open() override;
    void close() override;
    void finalize() override;

    void sample() override;

    void readSlot1(float *data) override;
    void readSlot2(uint32_t *data) override;

private:
    uint32_t _session;
    size_t _remaining;
};

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */

#endif /* EXPANSIONFPGA_H_ */
