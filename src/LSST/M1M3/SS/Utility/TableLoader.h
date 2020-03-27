#ifndef TABLELOADER_H_
#define TABLELOADER_H_

#include <Limit.h>
#include <DataTypes.h>
#include <SettingReader.h>
#include <boost/tokenizer.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/algorithm/string.hpp>
#include <fstream>
#include <string>
#include <vector>

namespace LSST {
namespace M1M3 {
namespace SS {

class TableLoader {
public:
    template <typename t>
    static void loadTable(int rowsToSkip, int columnsToSkip, int columnsToKeep, std::vector<t>* data,
                          const std::string& filename);
    static void loadLimitTable(int rowsToSkip, int columnsToSkip, std::vector<Limit>* data,
                               const std::string& filename);
};

template <typename t>
void TableLoader::loadTable(int rowsToSkip, int columnsToSkip, int columnsToKeep, std::vector<t>* data,
                            const std::string& filename) {
    typedef boost::tokenizer<boost::escaped_list_separator<char> > tokenizer;

    std::string fullPath = SettingReader::get().getFilePath(filename);
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
            for (int j = 0; j < columnsToKeep; j++) {
                data->push_back(boost::lexical_cast<t>(*i));
                ++i;
            }
        }
        lineNumber++;
    }
    inputStream.close();
}

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */

#endif /* TABLELOADER_H_ */
