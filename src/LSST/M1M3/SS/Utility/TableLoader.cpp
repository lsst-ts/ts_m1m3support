#include <TableLoader.h>

namespace LSST {
namespace M1M3 {
namespace SS {

void TableLoader::loadLimitTable(int rowsToSkip, int columnsToSkip, std::vector<Limit>* data,
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
