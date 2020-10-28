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

#include <ForceActuatorApplicationSettings.h>
#include <spdlog/spdlog.h>

#include <cstdlib>

namespace LSST {
namespace M1M3 {
namespace SS {

// Please see ForceActuatorTableRow documentation in
// ForceActuatorApplicationSettings.h for row structure.
// Each row contain ActuatorID, X, Y & Z Position, Type (Single/Dual), ILC
// Subnet & Address, and second axis orientation.
//
// If a change is needed, fill data from the new text file, delete actuator ID
// (as C++ memory structure forces row order to match order in the
// initialization, this is redundant value), add leading/ending {} and change
// Type and Orientation rows to include ForceActuatorTypes:: and
// ForceActuatorOrientations:: namespaces (struct).

//! [Table initialization]
ForceActuatorTableRow ForceActuatorApplicationSettings::Table[FA_COUNT] = {
        {101, 0.776782776, 0, -2.158743, ForceActuatorTypes::SAA, 3, 1, ForceActuatorOrientations::NA},
        {102, 1.442567993, 0, -2.158743, ForceActuatorTypes::DAA, 1, 17,
         ForceActuatorOrientations::PositiveY},
        {103, 2.10837793, 0, -2.158743, ForceActuatorTypes::DAA, 4, 17, ForceActuatorOrientations::PositiveY},
        {104, 2.774187988, 0, -2.158743, ForceActuatorTypes::DAA, 2, 17,
         ForceActuatorOrientations::PositiveY},
        {105, 3.439998047, 0, -2.158743, ForceActuatorTypes::DAA, 3, 17,
         ForceActuatorOrientations::PositiveY},
        {106, 3.968012939, 0, -2.158743, ForceActuatorTypes::SAA, 2, 1, ForceActuatorOrientations::NA},
        {107, 0.44386499, -0.57660498, -2.158743, ForceActuatorTypes::SAA, 1, 1,
         ForceActuatorOrientations::NA},
        {108, 1.109675049, -0.57660498, -2.158743, ForceActuatorTypes::DAA, 4, 18,
         ForceActuatorOrientations::PositiveY},
        {109, 1.775484985, -0.57660498, -2.158743, ForceActuatorTypes::DAA, 2, 18,
         ForceActuatorOrientations::PositiveY},
        {110, 2.441295898, -0.57660498, -2.158743, ForceActuatorTypes::DAA, 3, 18,
         ForceActuatorOrientations::PositiveY},
        {111, 3.107080078, -0.57660498, -2.158743, ForceActuatorTypes::DAA, 1, 18,
         ForceActuatorOrientations::PositiveY},
        {112, 3.772891113, -0.57660498, -2.158743, ForceActuatorTypes::DAA, 4, 19,
         ForceActuatorOrientations::NegativeX},
        {113, 0, -1.153209961, -2.158743, ForceActuatorTypes::DAA, 2, 19,
         ForceActuatorOrientations::PositiveY},
        {114, 0.776782776, -1.153209961, -2.158743, ForceActuatorTypes::DAA, 3, 19,
         ForceActuatorOrientations::PositiveY},
        {115, 1.442567993, -1.153209961, -2.158743, ForceActuatorTypes::DAA, 1, 19,
         ForceActuatorOrientations::PositiveY},
        {116, 2.10837793, -1.153209961, -2.158743, ForceActuatorTypes::DAA, 4, 20,
         ForceActuatorOrientations::PositiveY},
        {117, 2.774187988, -1.153209961, -2.158743, ForceActuatorTypes::DAA, 2, 20,
         ForceActuatorOrientations::PositiveY},
        {118, 3.439998047, -1.153209961, -2.158743, ForceActuatorTypes::DAA, 3, 20,
         ForceActuatorOrientations::PositiveY},
        {119, 3.9005, -0.997687012, -2.158743, ForceActuatorTypes::SAA, 2, 2, ForceActuatorOrientations::NA},
        {120, 0.44386499, -1.729819946, -2.158743, ForceActuatorTypes::DAA, 1, 20,
         ForceActuatorOrientations::PositiveY},
        {121, 1.109675049, -1.729819946, -2.158743, ForceActuatorTypes::DAA, 4, 21,
         ForceActuatorOrientations::PositiveY},
        {122, 1.775484985, -1.729819946, -2.158743, ForceActuatorTypes::DAA, 2, 21,
         ForceActuatorOrientations::PositiveY},
        {123, 2.44127002, -1.729819946, -2.158743, ForceActuatorTypes::DAA, 3, 21,
         ForceActuatorOrientations::PositiveY},
        {124, 3.107080078, -1.729819946, -2.158743, ForceActuatorTypes::DAA, 1, 21,
         ForceActuatorOrientations::PositiveY},
        {125, 3.724452881, -1.517949951, -2.158743, ForceActuatorTypes::SAA, 4, 1,
         ForceActuatorOrientations::NA},
        {126, 0, -2.306419922, -2.158743, ForceActuatorTypes::DAA, 2, 22,
         ForceActuatorOrientations::PositiveY},
        {127, 0.776782776, -2.306419922, -2.158743, ForceActuatorTypes::DAA, 3, 22,
         ForceActuatorOrientations::PositiveY},
        {128, 1.442567993, -2.306419922, -2.158743, ForceActuatorTypes::DAA, 1, 22,
         ForceActuatorOrientations::NegativeX},
        {129, 2.10837793, -2.306419922, -2.158743, ForceActuatorTypes::DAA, 4, 22,
         ForceActuatorOrientations::PositiveY},
        {130, 2.774187988, -2.306419922, -2.158743, ForceActuatorTypes::DAA, 2, 23,
         ForceActuatorOrientations::PositiveY},
        {131, 3.387954102, -2.167409912, -2.158743, ForceActuatorTypes::SAA, 3, 2,
         ForceActuatorOrientations::NA},
        {132, 0.44386499, -2.883030029, -2.158743, ForceActuatorTypes::DAA, 1, 23,
         ForceActuatorOrientations::PositiveY},
        {133, 1.109675049, -2.883030029, -2.158743, ForceActuatorTypes::DAA, 4, 23,
         ForceActuatorOrientations::PositiveY},
        {134, 1.775484985, -2.883030029, -2.158743, ForceActuatorTypes::DAA, 2, 24,
         ForceActuatorOrientations::PositiveY},
        {135, 2.44127002, -2.883030029, -2.158743, ForceActuatorTypes::DAA, 3, 23,
         ForceActuatorOrientations::NegativeX},
        {136, 2.939364014, -2.745179932, -2.158743, ForceActuatorTypes::SAA, 4, 2,
         ForceActuatorOrientations::NA},
        {137, 0.221945206, -3.459629883, -2.158743, ForceActuatorTypes::DAA, 2, 25,
         ForceActuatorOrientations::PositiveY},
        {138, 0.88772998, -3.459629883, -2.158743, ForceActuatorTypes::DAA, 3, 24,
         ForceActuatorOrientations::PositiveY},
        {139, 1.553540039, -3.267429932, -2.158743, ForceActuatorTypes::SAA, 1, 2,
         ForceActuatorOrientations::NA},
        {140, 2.089733887, -3.436389893, -2.158743, ForceActuatorTypes::SAA, 4, 3,
         ForceActuatorOrientations::NA},
        {141, 0.365734589, -4.00525, -2.158743, ForceActuatorTypes::SAA, 1, 3, ForceActuatorOrientations::NA},
        {142, 1.085088013, -3.87276001, -2.158743, ForceActuatorTypes::SAA, 2, 3,
         ForceActuatorOrientations::NA},
        {143, 1.60401001, -3.692780029, -2.158743, ForceActuatorTypes::SAA, 3, 3,
         ForceActuatorOrientations::NA},
        {207, -0.44386499, -0.57660498, -2.158743, ForceActuatorTypes::SAA, 1, 4,
         ForceActuatorOrientations::NA},
        {208, -1.109680054, -0.57660498, -2.158743, ForceActuatorTypes::DAA, 4, 24,
         ForceActuatorOrientations::PositiveY},
        {209, -1.77548999, -0.57660498, -2.158743, ForceActuatorTypes::DAA, 2, 26,
         ForceActuatorOrientations::PositiveY},
        {210, -2.441300049, -0.57660498, -2.158743, ForceActuatorTypes::DAA, 3, 25,
         ForceActuatorOrientations::PositiveY},
        {211, -3.107080078, -0.57660498, -2.158743, ForceActuatorTypes::DAA, 1, 24,
         ForceActuatorOrientations::PositiveY},
        {212, -3.772889893, -0.57660498, -2.158743, ForceActuatorTypes::DAA, 4, 25,
         ForceActuatorOrientations::PositiveX},
        {214, -0.77678302, -1.153209961, -2.158743, ForceActuatorTypes::DAA, 3, 26,
         ForceActuatorOrientations::PositiveY},
        {215, -1.442569946, -1.153209961, -2.158743, ForceActuatorTypes::DAA, 1, 25,
         ForceActuatorOrientations::PositiveY},
        {216, -2.108379883, -1.153209961, -2.158743, ForceActuatorTypes::DAA, 4, 26,
         ForceActuatorOrientations::PositiveY},
        {217, -2.774189941, -1.153209961, -2.158743, ForceActuatorTypes::DAA, 2, 27,
         ForceActuatorOrientations::PositiveY},
        {218, -3.44, -1.153209961, -2.158743, ForceActuatorTypes::DAA, 3, 27,
         ForceActuatorOrientations::PositiveY},
        {219, -3.9005, -0.997687012, -2.158743, ForceActuatorTypes::SAA, 2, 4, ForceActuatorOrientations::NA},
        {220, -0.44386499, -1.729819946, -2.158743, ForceActuatorTypes::DAA, 1, 26,
         ForceActuatorOrientations::PositiveY},
        {221, -1.109680054, -1.729819946, -2.158743, ForceActuatorTypes::DAA, 4, 27,
         ForceActuatorOrientations::PositiveY},
        {222, -1.77548999, -1.729819946, -2.158743, ForceActuatorTypes::DAA, 2, 28,
         ForceActuatorOrientations::PositiveY},
        {223, -2.44127002, -1.729819946, -2.158743, ForceActuatorTypes::DAA, 3, 28,
         ForceActuatorOrientations::PositiveY},
        {224, -3.107080078, -1.729819946, -2.158743, ForceActuatorTypes::DAA, 1, 27,
         ForceActuatorOrientations::PositiveY},
        {225, -3.724449951, -1.517949951, -2.158743, ForceActuatorTypes::SAA, 4, 4,
         ForceActuatorOrientations::NA},
        {227, -0.77678302, -2.306419922, -2.158743, ForceActuatorTypes::DAA, 3, 29,
         ForceActuatorOrientations::PositiveY},
        {228, -1.442569946, -2.306419922, -2.158743, ForceActuatorTypes::DAA, 1, 28,
         ForceActuatorOrientations::PositiveX},
        {229, -2.108379883, -2.306419922, -2.158743, ForceActuatorTypes::DAA, 4, 28,
         ForceActuatorOrientations::PositiveY},
        {230, -2.774189941, -2.306419922, -2.158743, ForceActuatorTypes::DAA, 2, 29,
         ForceActuatorOrientations::PositiveY},
        {231, -3.387949951, -2.167409912, -2.158743, ForceActuatorTypes::SAA, 3, 4,
         ForceActuatorOrientations::NA},
        {232, -0.44386499, -2.883030029, -2.158743, ForceActuatorTypes::DAA, 1, 29,
         ForceActuatorOrientations::PositiveY},
        {233, -1.109680054, -2.883030029, -2.158743, ForceActuatorTypes::DAA, 4, 29,
         ForceActuatorOrientations::PositiveY},
        {234, -1.77548999, -2.883030029, -2.158743, ForceActuatorTypes::DAA, 2, 30,
         ForceActuatorOrientations::PositiveY},
        {235, -2.44127002, -2.883030029, -2.158743, ForceActuatorTypes::DAA, 3, 30,
         ForceActuatorOrientations::PositiveX},
        {236, -2.939360107, -2.745179932, -2.158743, ForceActuatorTypes::SAA, 4, 5,
         ForceActuatorOrientations::NA},
        {237, -0.221945007, -3.459629883, -2.158743, ForceActuatorTypes::DAA, 2, 31,
         ForceActuatorOrientations::PositiveY},
        {238, -0.88772998, -3.459629883, -2.158743, ForceActuatorTypes::DAA, 3, 31,
         ForceActuatorOrientations::PositiveY},
        {239, -1.553540039, -3.267429932, -2.158743, ForceActuatorTypes::SAA, 1, 5,
         ForceActuatorOrientations::NA},
        {240, -2.08972998, -3.436389893, -2.158743, ForceActuatorTypes::SAA, 4, 6,
         ForceActuatorOrientations::NA},
        {241, -0.365734985, -4.00525, -2.158743, ForceActuatorTypes::SAA, 1, 6,
         ForceActuatorOrientations::NA},
        {242, -1.085089966, -3.87276001, -2.158743, ForceActuatorTypes::SAA, 2, 5,
         ForceActuatorOrientations::NA},
        {243, -1.60401001, -3.692780029, -2.158743, ForceActuatorTypes::SAA, 3, 5,
         ForceActuatorOrientations::NA},
        {301, -0.77678302, 0, -2.158743, ForceActuatorTypes::SAA, 3, 6, ForceActuatorOrientations::NA},
        {302, -1.442569946, 0, -2.158743, ForceActuatorTypes::DAA, 1, 30,
         ForceActuatorOrientations::PositiveY},
        {303, -2.108379883, 0, -2.158743, ForceActuatorTypes::DAA, 4, 30,
         ForceActuatorOrientations::PositiveY},
        {304, -2.774189941, 0, -2.158743, ForceActuatorTypes::DAA, 2, 32,
         ForceActuatorOrientations::PositiveY},
        {305, -3.44, 0, -2.158743, ForceActuatorTypes::DAA, 3, 32, ForceActuatorOrientations::PositiveY},
        {306, -3.96801001, 0, -2.158743, ForceActuatorTypes::SAA, 2, 6, ForceActuatorOrientations::NA},
        {307, -0.44386499, 0.576605408, -2.158743, ForceActuatorTypes::SAA, 1, 7,
         ForceActuatorOrientations::NA},
        {308, -1.109680054, 0.576605408, -2.158743, ForceActuatorTypes::DAA, 4, 31,
         ForceActuatorOrientations::PositiveY},
        {309, -1.77548999, 0.576605408, -2.158743, ForceActuatorTypes::DAA, 2, 33,
         ForceActuatorOrientations::PositiveY},
        {310, -2.441300049, 0.576605408, -2.158743, ForceActuatorTypes::DAA, 3, 33,
         ForceActuatorOrientations::PositiveY},
        {311, -3.107080078, 0.576605408, -2.158743, ForceActuatorTypes::DAA, 1, 31,
         ForceActuatorOrientations::NegativeY},
        {312, -3.772889893, 0.576605408, -2.158743, ForceActuatorTypes::DAA, 4, 32,
         ForceActuatorOrientations::PositiveX},
        {313, 0, 1.15321106, -2.158743, ForceActuatorTypes::DAA, 2, 34, ForceActuatorOrientations::PositiveY},
        {314, -0.77678302, 1.15321106, -2.158743, ForceActuatorTypes::DAA, 3, 34,
         ForceActuatorOrientations::PositiveY},
        {315, -1.442569946, 1.15321106, -2.158743, ForceActuatorTypes::DAA, 1, 32,
         ForceActuatorOrientations::PositiveY},
        {316, -2.108379883, 1.15321106, -2.158743, ForceActuatorTypes::DAA, 4, 33,
         ForceActuatorOrientations::PositiveY},
        {317, -2.774189941, 1.15321106, -2.158743, ForceActuatorTypes::DAA, 2, 35,
         ForceActuatorOrientations::PositiveY},
        {318, -3.44, 1.15321106, -2.158743, ForceActuatorTypes::DAA, 3, 35,
         ForceActuatorOrientations::PositiveY},
        {319, -3.9005, 0.997686584, -2.158743, ForceActuatorTypes::SAA, 2, 7, ForceActuatorOrientations::NA},
        {320, -0.44386499, 1.72981604, -2.158743, ForceActuatorTypes::DAA, 1, 33,
         ForceActuatorOrientations::PositiveY},
        {321, -1.109680054, 1.72981604, -2.158743, ForceActuatorTypes::DAA, 4, 34,
         ForceActuatorOrientations::PositiveY},
        {322, -1.77548999, 1.72981604, -2.158743, ForceActuatorTypes::DAA, 2, 36,
         ForceActuatorOrientations::PositiveY},
        {323, -2.44127002, 1.72981604, -2.158743, ForceActuatorTypes::DAA, 3, 36,
         ForceActuatorOrientations::PositiveY},
        {324, -3.107080078, 1.72981604, -2.158743, ForceActuatorTypes::DAA, 1, 34,
         ForceActuatorOrientations::PositiveY},
        {325, -3.724449951, 1.517954956, -2.158743, ForceActuatorTypes::SAA, 4, 7,
         ForceActuatorOrientations::NA},
        {326, 0, 2.306422119, -2.158743, ForceActuatorTypes::DAA, 2, 37,
         ForceActuatorOrientations::PositiveY},
        {327, -0.77678302, 2.306422119, -2.158743, ForceActuatorTypes::DAA, 3, 37,
         ForceActuatorOrientations::PositiveY},
        {328, -1.442569946, 2.306422119, -2.158743, ForceActuatorTypes::DAA, 1, 35,
         ForceActuatorOrientations::PositiveX},
        {329, -2.108379883, 2.306422119, -2.158743, ForceActuatorTypes::DAA, 4, 35,
         ForceActuatorOrientations::PositiveY},
        {330, -2.774189941, 2.306422119, -2.158743, ForceActuatorTypes::DAA, 2, 38,
         ForceActuatorOrientations::PositiveY},
        {331, -3.387949951, 2.167406982, -2.158743, ForceActuatorTypes::SAA, 3, 7,
         ForceActuatorOrientations::NA},
        {332, -0.44386499, 2.8830271, -2.158743, ForceActuatorTypes::DAA, 1, 36,
         ForceActuatorOrientations::PositiveY},
        {333, -1.109680054, 2.8830271, -2.158743, ForceActuatorTypes::DAA, 4, 36,
         ForceActuatorOrientations::PositiveY},
        {334, -1.77548999, 2.8830271, -2.158743, ForceActuatorTypes::DAA, 2, 39,
         ForceActuatorOrientations::NegativeY},
        {335, -2.44127002, 2.8830271, -2.158743, ForceActuatorTypes::DAA, 3, 38,
         ForceActuatorOrientations::PositiveX},
        {336, -2.939360107, 2.745180908, -2.158743, ForceActuatorTypes::SAA, 4, 8,
         ForceActuatorOrientations::NA},
        {337, -0.221945007, 3.45963208, -2.158743, ForceActuatorTypes::DAA, 2, 40,
         ForceActuatorOrientations::PositiveY},
        {338, -0.88772998, 3.45963208, -2.158743, ForceActuatorTypes::DAA, 3, 39,
         ForceActuatorOrientations::PositiveY},
        {339, -1.553540039, 3.267430908, -2.158743, ForceActuatorTypes::SAA, 1, 8,
         ForceActuatorOrientations::NA},
        {340, -2.08972998, 3.436391113, -2.158743, ForceActuatorTypes::SAA, 4, 9,
         ForceActuatorOrientations::NA},
        {341, -0.365734985, 4.00525, -2.158743, ForceActuatorTypes::SAA, 1, 9, ForceActuatorOrientations::NA},
        {342, -1.085089966, 3.872762939, -2.158743, ForceActuatorTypes::SAA, 2, 8,
         ForceActuatorOrientations::NA},
        {343, -1.60401001, 3.692779053, -2.158743, ForceActuatorTypes::SAA, 3, 8,
         ForceActuatorOrientations::NA},
        {407, 0.44386499, 0.576605408, -2.158743, ForceActuatorTypes::SAA, 1, 10,
         ForceActuatorOrientations::NA},
        {408, 1.109675049, 0.576605408, -2.158743, ForceActuatorTypes::DAA, 4, 37,
         ForceActuatorOrientations::PositiveY},
        {409, 1.775484985, 0.576605408, -2.158743, ForceActuatorTypes::DAA, 2, 41,
         ForceActuatorOrientations::PositiveY},
        {410, 2.441295898, 0.576605408, -2.158743, ForceActuatorTypes::DAA, 3, 40,
         ForceActuatorOrientations::PositiveY},
        {411, 3.107080078, 0.576605408, -2.158743, ForceActuatorTypes::DAA, 1, 37,
         ForceActuatorOrientations::NegativeY},
        {412, 3.772891113, 0.576605408, -2.158743, ForceActuatorTypes::DAA, 4, 38,
         ForceActuatorOrientations::NegativeX},
        {414, 0.776782776, 1.15321106, -2.158743, ForceActuatorTypes::DAA, 3, 41,
         ForceActuatorOrientations::PositiveY},
        {415, 1.442567993, 1.15321106, -2.158743, ForceActuatorTypes::DAA, 1, 38,
         ForceActuatorOrientations::PositiveY},
        {416, 2.10837793, 1.15321106, -2.158743, ForceActuatorTypes::DAA, 4, 39,
         ForceActuatorOrientations::PositiveY},
        {417, 2.774187988, 1.15321106, -2.158743, ForceActuatorTypes::DAA, 2, 42,
         ForceActuatorOrientations::PositiveY},
        {418, 3.439998047, 1.15321106, -2.158743, ForceActuatorTypes::DAA, 3, 42,
         ForceActuatorOrientations::PositiveY},
        {419, 3.9005, 0.997686584, -2.158743, ForceActuatorTypes::SAA, 2, 9, ForceActuatorOrientations::NA},
        {420, 0.44386499, 1.72981604, -2.158743, ForceActuatorTypes::DAA, 1, 39,
         ForceActuatorOrientations::PositiveY},
        {421, 1.109675049, 1.72981604, -2.158743, ForceActuatorTypes::DAA, 4, 40,
         ForceActuatorOrientations::PositiveY},
        {422, 1.775484985, 1.72981604, -2.158743, ForceActuatorTypes::DAA, 2, 43,
         ForceActuatorOrientations::PositiveY},
        {423, 2.44127002, 1.72981604, -2.158743, ForceActuatorTypes::DAA, 3, 43,
         ForceActuatorOrientations::PositiveY},
        {424, 3.107080078, 1.72981604, -2.158743, ForceActuatorTypes::DAA, 1, 40,
         ForceActuatorOrientations::PositiveY},
        {425, 3.724452881, 1.517954956, -2.158743, ForceActuatorTypes::SAA, 4, 10,
         ForceActuatorOrientations::NA},
        {427, 0.776782776, 2.306422119, -2.158743, ForceActuatorTypes::DAA, 3, 44,
         ForceActuatorOrientations::PositiveY},
        {428, 1.442567993, 2.306422119, -2.158743, ForceActuatorTypes::DAA, 1, 41,
         ForceActuatorOrientations::NegativeX},
        {429, 2.10837793, 2.306422119, -2.158743, ForceActuatorTypes::DAA, 4, 41,
         ForceActuatorOrientations::PositiveY},
        {430, 2.774187988, 2.306422119, -2.158743, ForceActuatorTypes::DAA, 2, 44,
         ForceActuatorOrientations::PositiveY},
        {431, 3.387954102, 2.167406982, -2.158743, ForceActuatorTypes::SAA, 3, 9,
         ForceActuatorOrientations::NA},
        {432, 0.44386499, 2.8830271, -2.158743, ForceActuatorTypes::DAA, 1, 42,
         ForceActuatorOrientations::PositiveY},
        {433, 1.109675049, 2.8830271, -2.158743, ForceActuatorTypes::DAA, 4, 42,
         ForceActuatorOrientations::PositiveY},
        {434, 1.775484985, 2.8830271, -2.158743, ForceActuatorTypes::DAA, 2, 45,
         ForceActuatorOrientations::NegativeY},
        {435, 2.44127002, 2.8830271, -2.158743, ForceActuatorTypes::DAA, 3, 45,
         ForceActuatorOrientations::NegativeX},
        {436, 2.939364014, 2.745180908, -2.158743, ForceActuatorTypes::SAA, 4, 11,
         ForceActuatorOrientations::NA},
        {437, 0.221945206, 3.45963208, -2.158743, ForceActuatorTypes::DAA, 2, 46,
         ForceActuatorOrientations::PositiveY},
        {438, 0.88772998, 3.45963208, -2.158743, ForceActuatorTypes::DAA, 3, 46,
         ForceActuatorOrientations::PositiveY},
        {439, 1.553540039, 3.267430908, -2.158743, ForceActuatorTypes::SAA, 1, 11,
         ForceActuatorOrientations::NA},
        {440, 2.089733887, 3.436391113, -2.158743, ForceActuatorTypes::SAA, 4, 12,
         ForceActuatorOrientations::NA},
        {441, 0.365734589, 4.00525, -2.158743, ForceActuatorTypes::SAA, 1, 12, ForceActuatorOrientations::NA},
        {442, 1.085088013, 3.872762939, -2.158743, ForceActuatorTypes::SAA, 2, 10,
         ForceActuatorOrientations::NA},
        {443, 1.60401001, 3.692779053, -2.158743, ForceActuatorTypes::SAA, 3, 10,
         ForceActuatorOrientations::NA}};
//! [Table initialization]

ForceActuatorApplicationSettings::ForceActuatorApplicationSettings() {
    // fill helpers tables
    int xIndex = 0;
    int yIndex = 0;
    int sIndex = 0;
    for (int zIndex = 0; zIndex < FA_COUNT; ++zIndex) {
        ForceActuatorTypes type = Table[zIndex].Type;
        ForceActuatorOrientations orientation = Table[zIndex].Orientation;
        if (type == ForceActuatorTypes::DAA) {
            if (orientation == ForceActuatorOrientations::PositiveX ||
                orientation == ForceActuatorOrientations::NegativeX) {
                XIndexToZIndex[xIndex] = zIndex;

                ZIndexToXIndex[zIndex] = xIndex;
                ZIndexToYIndex[zIndex] = -1;

                xIndex += 1;
            }

            if (orientation == ForceActuatorOrientations::PositiveY ||
                orientation == ForceActuatorOrientations::NegativeY) {
                YIndexToZIndex[yIndex] = zIndex;

                ZIndexToXIndex[zIndex] = -1;
                ZIndexToYIndex[zIndex] = yIndex;

                yIndex += 1;
            }

            if (orientation != ForceActuatorOrientations::NA) {
                SecondaryCylinderIndexToZIndex[sIndex] = zIndex;

                ZIndexToSecondaryCylinderIndex[zIndex] = sIndex;

                sIndex += 1;
            }
        } else {
            ZIndexToXIndex[zIndex] = -1;
            ZIndexToYIndex[zIndex] = -1;
            ZIndexToSecondaryCylinderIndex[zIndex] = -1;
        }
    }
    if ((xIndex != FA_X_COUNT) || (yIndex != FA_Y_COUNT) || (sIndex != (FA_X_COUNT + FA_Y_COUNT))) {
        spdlog::critical(
                "ForceActuatorApplicationSettings::ForceActuatorApplicationSettings invalid number of X or Y "
                "actuators: x {} y {} s {}",
                xIndex, yIndex, sIndex);
        exit(EXIT_FAILURE);
    }
}

const int ForceActuatorApplicationSettings::ActuatorIdToZIndex(int actuatorId) {
    for (int zIndex = 0; zIndex < FA_COUNT; ++zIndex) {
        if (Table[zIndex].ActuatorID == actuatorId) return zIndex;
    }
    return -1;
}

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */
