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

#ifndef DATATYPES_H_
#define DATATYPES_H_

#include <stdint.h>

#define ACK_INPROGRESS 301  //* Acknowledges command reception, command is being executed.
#define ACK_COMPLETE 303    //* Command is completed.
#define ACK_FAILED -302     //* Command execution failed.

/** @brief Number of ModBus subnets.
 *
 * Subnets are connected and commanded through cRIO DIOs.
 */
#define SUBNET_COUNT 5
#define FA_COUNT 156
#define FA_X_COUNT 12
#define FA_Y_COUNT 100
#define FA_Z_COUNT 156
#define HP_COUNT 6
#define HM_COUNT 6
#define BENDING_MODES 22

#define MILLIMETERS_PER_METER 1000.0
#define MICROMETERS_PER_MILLIMETER 1000.0

// SAL__CMD_ACK        300
// SAL__CMD_INPROGRESS 301
// SAL__CMD_STALLED    302
// SAL__CMD_COMPLETE   303
// SAL__CMD_NOPERM    -300
// SAL__CMD_NOACK     -301
// SAL__CMD_FAILED    -302
// SAL__CMD_ABORTED   -303
// SAL__CMD_TIMEOUT   -304

#endif /* DATATYPES_H_ */
