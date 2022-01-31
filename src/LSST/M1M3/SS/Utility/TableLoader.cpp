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

#include <TableLoader.h>

namespace LSST {
namespace M1M3 {
namespace SS {

void TableLoader::loadLimitTable(int rowsToSkip, int columnsToSkip, std::vector<Limit>* data,
                                 const std::string& filename) {
    typedef boost::tokenizer<boost::escaped_list_separator<char> > tokenizer;

    std::string fullPath = SettingReader::instance().getFilePath(filename);
    std::ifstream inputStream(fullPath.c_str());
    if (!inputStream.is_open()) {
        throw std::runtime_error("Cannot open " + fullPath + ": " + strerror(errno));
    }
    std::string lineText;
    int32_t lineNumber = 0;
    data->clear();
    while (std::getline(inputStream, lineText)) {
        boost::trim_right(lineText);
        if (lineNumber >= rowsToSkip && !lineText.empty()) {
            tokenizer tok(lineText);
            tokenizer::iterator i = tok.begin();
            for (int j = 0; j < columnsToSkip; j++) {
                ++i;
            }
            Limit limit;
            limit.LowFault = boost::lexical_cast<float>(*i);
            ++i;
            limit.LowWarning = boost::lexical_cast<float>(*i);
            ++i;
            limit.HighWarning = boost::lexical_cast<float>(*i);
            ++i;
            limit.HighFault = boost::lexical_cast<float>(*i);
            data->push_back(limit);
        }
        lineNumber++;
    }
    inputStream.close();
}

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */
