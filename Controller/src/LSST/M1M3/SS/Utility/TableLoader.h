/*
 * TableLoader.h
 *
 *  Created on: Nov 16, 2017
 *      Author: ccontaxis
 */

#ifndef TABLELOADER_H_
#define TABLELOADER_H_

#include <Limit.h>
#include <DataTypes.h>
#include <string>
#include <vector>

namespace LSST {
namespace M1M3 {
namespace SS {

class TableLoader {
public:
	static void loadTable(int rowsToSkip, int columnsToSkip, int columnsToKeep, std::vector<double>* data, const std::string &filename);
	static void loadTable(int rowsToSkip, int columnsToSkip, int columnsToKeep, std::vector<float>* data, const std::string &filename);
	static void loadTable(int rowsToSkip, int columnsToSkip, int columnsToKeep, std::vector<int32_t>* data, const std::string &filename);
	static void loadLimitTable(int rowsToSkip, int columnsToSkip, std::vector<Limit>* data, const std::string &filename);
};

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */

#endif /* TABLELOADER_H_ */
