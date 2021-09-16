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

#include <IBusList.h>

namespace LSST {
namespace M1M3 {
namespace SS {

IBusList::~IBusList() {}

int32_t IBusList::getLength() { return 0; }
uint16_t* IBusList::getBuffer() { return 0; }

int32_t* IBusList::getExpectedHPResponses() { return 0; }
int32_t* IBusList::getExpectedFAResponses() { return 0; }
int32_t* IBusList::getExpectedHMResponses() { return 0; }

void IBusList::update() {}

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */