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

#ifndef LSST_M1M3_SS_FPGA_NiError_H_
#define LSST_M1M3_SS_FPGA_NiError_H_

#include "NiFpga.h"
#include <stdexcept>

/**
 * Error thrown on FPGA problems. Provides what routine with explanatory
 * string.
 */
class NiError : public std::runtime_error {
public:
    /**
     * Construct the error.
     *
     * @param status Ni error status.
     */
    NiError(const char *msg, NiFpga_Status status);
};

/**
 * Throws NI exception if an error occurred.
 */
void NiThrowError(const char *msg, int32_t status);

#endif  // !LSST_M1M3_SS_FPGA_NiError_H_
