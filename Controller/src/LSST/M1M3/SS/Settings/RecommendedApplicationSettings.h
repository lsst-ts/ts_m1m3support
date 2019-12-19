/*
 * RecommendedApplicationSettings.h
 *
 *  Created on: Sep 29, 2017
 *      Author: ccontaxis
 */

#ifndef RECOMMENDEDAPPLICATIONSETTINGS_H_
#define RECOMMENDEDAPPLICATIONSETTINGS_H_

#include <string>
#include <vector>

namespace LSST {
namespace M1M3 {
namespace SS {

struct RecommendedApplicationSettings {
    std::vector<std::string> RecommendedSettings;

    void load(const std::string& filename);
};

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */

#endif /* RECOMMENDEDAPPLICATIONSETTINGS_H_ */
