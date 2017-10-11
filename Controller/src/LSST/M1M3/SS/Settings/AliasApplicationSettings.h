/*
 * AliasApplicationSettings.h
 *
 *  Created on: Sep 29, 2017
 *      Author: ccontaxis
 */

#ifndef ALIASAPPLICATIONSETTINGS_H_
#define ALIASAPPLICATIONSETTINGS_H_

#include <string>
#include <vector>

namespace LSST {
namespace M1M3 {
namespace SS {

struct Alias {
	std::string Name;
	std::string Set;
	std::string Version;
};

struct AliasApplicationSettings {
	std::vector<Alias> Aliases;

	void load(const std::string &filename);
};

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */

#endif /* ALIASAPPLICATIONSETTINGS_H_ */
