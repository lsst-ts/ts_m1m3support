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

#ifndef TABLELOADER_H_
#define TABLELOADER_H_

#include <string>
#include <vector>

#include <spdlog/fmt/fmt.h>

#include <rapidcsv.h>

#include <Limit.h>
#include <DataTypes.h>
#include <SettingReader.h>

namespace LSST {
namespace M1M3 {
namespace SS {

/// Conversion functions, used when worng data are encountered
std::string rowToStr(std::vector<std::string> row);

class TableLoader {
public:
    template <typename t>
    static void loadTable(size_t columnsToSkip, size_t columnsToKeep, std::vector<t>* data,
                          const std::string& filename);
    static void loadLimitTable(size_t columnsToSkip, std::vector<Limit>* data, const std::string& filename);
    static void loadMirrorLimitTable(size_t columnsToSkip, float zLow[FA_Z_COUNT], float zHigh[FA_Z_COUNT],
                                     float yLow[FA_Y_COUNT], float yHigh[FA_Y_COUNT], float xLow[FA_X_COUNT],
                                     float xHigh[FA_X_COUNT], const std::string& filename);
};

template <typename t>
void TableLoader::loadTable(size_t columnsToSkip, size_t columnsToKeep, std::vector<t>* data,
                            const std::string& filename) {
    std::string fullPath = SettingReader::instance().getFilePath(filename);
    try {
        rapidcsv::Document table(fullPath);
        data->clear();
        if (columnsToSkip + columnsToKeep != table.GetColumnCount()) {
            throw std::runtime_error(fmt::format("CSV {} has {} columns, expected {}", fullPath,
                                                 table.GetColumnCount(), columnsToSkip + columnsToKeep));
        }
        for (size_t row = 0; row < table.GetRowCount(); row++) {
            for (size_t column = columnsToSkip; column < columnsToSkip + columnsToKeep; column++) {
                try {
                    data->push_back(table.GetCell<t>(column, row));
                } catch (std::logic_error& er) {
                    throw std::runtime_error(fmt::format("{}:{}:{}: cannot parse {}: {}", fullPath, row,
                                                         column, table.GetCell<std::string>(column, row),
                                                         er.what()));
                }
            }
        }
    } catch (std::ios_base::failure& er) {
        throw std::runtime_error(fmt::format("Cannot read CSV {}: {}", fullPath, er.what()));
    }
}

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */

#endif /* TABLELOADER_H_ */
