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

#ifndef EXPANSIONFPGA_H_
#define EXPANSIONFPGA_H_

#include <IExpansionFPGA.h>

namespace LSST {
namespace M1M3 {
namespace SS {

class ExpansionFPGA : public IExpansionFPGA {
public:
    ExpansionFPGA();

    int32_t initialize() override;
    int32_t open() override;
    int32_t close() override;
    int32_t finalize() override;

    bool isErrorCode(int32_t status) override;

    int32_t sample() override;

    int32_t readSlot1(float* data) override;
    int32_t readSlot2(uint32_t* data) override;

private:
    uint32_t _session;
    size_t _remaining;
};

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */

#endif /* EXPANSIONFPGA_H_ */
