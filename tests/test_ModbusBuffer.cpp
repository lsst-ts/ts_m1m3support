/*
 * This file is part of LSST M1M3 SS test suite. Tests Modbus buffer class.
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

#define CATCH_CONFIG_MAIN
#include <catch/catch.hpp>

#include <memory>
#include <cmath>
#include <iostream>

#include <ModbusBuffer.h>

using namespace LSST::M1M3::SS;

TEST_CASE("CalculateCRC", "[ModbusBuffer]") {
    ModbusBuffer mbuf;
    // address
    mbuf.writeU8(123);
    mbuf.writeU8(17);
    mbuf.writeCRC(2);
    mbuf.writeEndOfFrame();

    uint16_t* buf = mbuf.getBuffer();

    REQUIRE(mbuf.readInstructionByte(buf[2]) == 0xe3);
    REQUIRE(mbuf.readInstructionByte(buf[3]) == 0x4c);
}
