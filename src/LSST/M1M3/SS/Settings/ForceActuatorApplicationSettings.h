/*
 * ForceActuatorApplicationSettings.h
 *
 *  Created on: Sep 29, 2017
 *      Author: ccontaxis
 */

#ifndef FORCEACTUATORAPPLICATIONSETTINGS_H_
#define FORCEACTUATORAPPLICATIONSETTINGS_H_

#include <ForceActuatorTableRow.h>
#include <string>
#include <vector>

namespace LSST {
namespace M1M3 {
namespace SS {

class ForceActuatorApplicationSettings {
public:
    std::vector<int32_t> XIndexToZIndex;
    std::vector<int32_t> YIndexToZIndex;
    std::vector<int32_t> SecondaryCylinderIndexToZIndex;
    std::vector<int32_t> ZIndexToXIndex;
    std::vector<int32_t> ZIndexToYIndex;
    std::vector<int32_t> ZIndexToSecondaryCylinderIndex;
    std::vector<ForceActuatorTableRow> Table;

    void load(const std::string &filename);

private:
    void loadForceActuatorTable(const std::string &filename);
};

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */

#endif /* FORCEACTUATORAPPLICATIONSETTINGS_H_ */
