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

#include <cRIO/NiStatus.h>

#include <HealthAndStatusFPGAData.h>

using namespace LSST::M1M3::SS;

ModbusPort::ModbusPort()
    : errorFlag(0), txBytes(0), txFrames(0), rxBytes(0), rxFrames(0),
      instructionCount(0) {}

void ModbusPort::fillFromFIFO(uint64_t *buffer) {
  errorFlag = buffer[0];
  txBytes = buffer[1];
  txFrames = buffer[2];
  rxBytes = buffer[3];
  rxFrames = buffer[4];
  instructionCount = buffer[5];
}

void HealthAndStatusFPGAData::refresh(
    uint64_t buffer[PORTNUM * PORT_COUNTNUM]) {
  for (int i = 0; i < PORTNUM; i++) {
    ports[i].fillFromFIFO(buffer + i * PORT_COUNTNUM);
  }
}
