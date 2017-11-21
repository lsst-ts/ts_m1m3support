/*
 * TableLoader.cpp
 *
 *  Created on: Nov 16, 2017
 *      Author: ccontaxis
 */

#include <TableLoader.h>
#include <pugixml.hpp>
#include <boost/tokenizer.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/algorithm/string.hpp>
#include <fstream>

namespace LSST {
namespace M1M3 {
namespace SS {

void TableLoader::loadTable(int rowsToSkip, int columnsToSkip, int columnsToKeep, std::vector<double>* data, const std::string &filename) {
	typedef boost::tokenizer< boost::escaped_list_separator<char> > tokenizer;
	std::ifstream inputStream(filename.c_str());
	std::string lineText;
	int32_t lineNumber = 0;
	while(std::getline(inputStream, lineText)) {
		boost::trim_right(lineText);
		if (lineNumber >= rowsToSkip && !lineText.empty()) {
			tokenizer tok(lineText);
			tokenizer::iterator i = tok.begin();
			for(int j = 0; j < columnsToSkip; j++) {
				++i;
			}
			for(int j = 0; j < columnsToKeep; j++) {
				data->push_back(boost::lexical_cast<double>(*i));
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
