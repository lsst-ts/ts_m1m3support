/*
 * TableLoader.h
 *
 *  Created on: Nov 16, 2017
 *      Author: ccontaxis
 */

#ifndef TABLELOADER_H_
#define TABLELOADER_H_

#include <string>
#include <vector>

namespace LSST {
namespace M1M3 {
namespace SS {

class TableLoader {
public:
	static void loadTable(int rowsToSkip, int columnsToSkip, int columnsToKeep, std::vector<double>* data, const std::string &filename);
};

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */

#endif /* TABLELOADER_H_ */
