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

#ifndef NiStatus_H_
#define NiStatus_H_

#include <stdint.h>

/**
 * Converts NI error to string description. See
 * http://zone.ni.com/reference/en-XX/help/372928H-01/capi/errors/ for error
 * codes.
 *
 * @param status status to convert
 *
 * @return error description, or "Unknow error status" if not known
 */
const char* NiStatus(int32_t status);

/**
 * Log NiFpga call error.
 *
 * @param msg additional message (can be __PRETTY_FUNCTION__)
 * @param status NiFpga returned status
 *
 * @return status (so it can be reused)
 */
int32_t NiReportError(const char* msg, int32_t status);

#endif /* NiStatus_H_ */
