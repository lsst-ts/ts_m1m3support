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

#include <TableLoader.h>

using namespace LSST::M1M3::SS;

std::string LSST::M1M3::SS::rowToStr(std::vector<std::string> row) {
    std::string ret;
    for (auto it = row.begin(); it != row.end(); it++) {
        ret += (it == row.begin() ? "" : ", ") + *it;
    }
    return ret;
}

void TableLoader::loadLimitTable(size_t columnsToSkip, std::vector<Limit>* data,
                                 const std::string& filename) {
    std::string fullPath = SettingReader::instance().getFilePath(filename);
    try {
        rapidcsv::Document limitTable(fullPath);
        data->clear();
        if (columnsToSkip + 4 != limitTable.GetColumnCount()) {
            throw std::runtime_error(fmt::format("CSV {} has {} columns, expected {}", fullPath,
                                                 limitTable.GetColumnCount(), columnsToSkip + 4));
        }
        for (size_t row = 0; row < limitTable.GetRowCount(); row++) {
            try {
                data->push_back(Limit(limitTable.GetCell<float>(columnsToSkip, row),
                                      limitTable.GetCell<float>(columnsToSkip + 1, row),
                                      limitTable.GetCell<float>(columnsToSkip + 2, row),
                                      limitTable.GetCell<float>(columnsToSkip + 3, row)));
            } catch (std::logic_error& er) {
                throw std::runtime_error(fmt::format("{}:{}: cannot parse row (\"{}\"): {}", fullPath, row,
                                                     rowToStr(limitTable.GetRow<std::string>(row)),
                                                     er.what()));
            }
        }
    } catch (std::ios_base::failure& er) {
        throw std::runtime_error(fmt::format("Cannot read CSV {}: {}", fullPath, er.what()));
    }
}

void TableLoader::loadCylinderLimitTable(size_t columnsToSkip, float primaryLow[FA_COUNT],
                                         float primaryHigh[FA_COUNT], float secondaryLow[FA_S_COUNT],
                                         float secondaryHigh[FA_S_COUNT], const std::string& filename) {
    std::string fullPath = SettingReader::instance().getFilePath(filename);
    try {
        rapidcsv::Document limitTable(fullPath);
        if (columnsToSkip + 4 != limitTable.GetColumnCount()) {
            throw std::runtime_error(fmt::format("CSV {} has {} columns, expected {}", fullPath,
                                                 limitTable.GetColumnCount(), columnsToSkip + 4));
        }
        if (limitTable.GetRowCount() != FA_COUNT) {
            throw std::runtime_error(fmt::format("CSV {} has {} rows, expected {}", fullPath,
                                                 limitTable.GetRowCount(), FA_COUNT));
        }
        for (size_t row = 0; row < FA_COUNT; row++) {
            auto id = limitTable.GetCell<int>(0, row);
            auto expectedId =
                    SettingReader::instance().getForceActuatorApplicationSettings()->ZIndexToActuatorId(row);
            if (id != expectedId) {
                throw std::runtime_error(
                        fmt::format("CSV {} row {} has incorrect ActuatorID - {}, expected {}", fullPath, row,
                                    id, expectedId));
            }
            primaryLow[row] = limitTable.GetCell<float>(1, row);
            primaryHigh[row] = limitTable.GetCell<float>(2, row);
            if (primaryLow[row] >= primaryHigh[row]) {
                throw std::runtime_error(fmt::format(
                        "CSV {} row {} - primary limits are in incorrect order, low - high expected, {} - {} "
                        "configured.",
                        fullPath, row, primaryLow[row], primaryHigh[row]));
            }

            auto secIndex = SettingReader::instance()
                                    .getForceActuatorApplicationSettings()
                                    ->ZIndexToSecondaryCylinderIndex[row];
            if (secIndex == -1) {
                auto secLow = limitTable.GetCell<std::string>(3, row);
                auto secHigh = limitTable.GetCell<std::string>(4, row);
                if (secLow != "-" || secHigh != "-") {
                    throw std::runtime_error(
                            fmt::format("CSV {} row {} shouldn't specify secondary limits (should be '-'), "
                                        "yet {} and {} found.",
                                        fullPath, row, secLow, secHigh));
                }
            } else {
                secondaryLow[secIndex] = limitTable.GetCell<float>(3, row);
                secondaryHigh[secIndex] = limitTable.GetCell<float>(4, row);
                if (secondaryLow[secIndex] >= secondaryHigh[secIndex]) {
                    throw std::runtime_error(
                            fmt::format("CSV {} row {} - secondary limits are in incorrect order, low - high "
                                        "expected, {} - {} configured.",
                                        fullPath, row, secondaryLow[secIndex], secondaryHigh[secIndex]));
                }
            }
        }
    } catch (std::ios_base::failure& er) {
        throw std::runtime_error(fmt::format("Cannot read CSV {}: {}", fullPath, er.what()));
    }
}
