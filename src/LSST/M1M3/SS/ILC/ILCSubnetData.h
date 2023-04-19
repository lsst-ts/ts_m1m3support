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

#ifndef ILCSUBNETDATA_H_
#define ILCSUBNETDATA_H_

#include <ILCDataTypes.h>

namespace LSST {
namespace M1M3 {
namespace SS {

class ForceActuatorApplicationSettings;
class ForceActuatorSettings;
class HardpointActuatorApplicationSettings;
class HardpointMonitorApplicationSettings;

class ILCSubnetData {
    struct Container {
        int32_t HPCount;
        std::vector<ILCMap> HPIndex;
        int32_t FACount;
        std::vector<ILCMap> FAIndex;
        int32_t HMCount;
        std::vector<ILCMap> HMIndex;
        ILCMap ILCDataFromAddress[256];
    };
    Container subnetData[5];

public:
    ILCSubnetData(ForceActuatorApplicationSettings* forceActuatorApplicationSettings,
                  HardpointActuatorApplicationSettings* hardpointActuatorApplicationSettings,
                  HardpointMonitorApplicationSettings* hardpointMonitorApplicationSettings);

    int32_t getHPCount(int32_t subnetIndex) { return this->subnetData[subnetIndex].HPIndex.size(); }
    ILCMap getHPIndex(int32_t subnetIndex, int32_t hpIndex) {
        return this->subnetData[subnetIndex].HPIndex[hpIndex];
    }
    int32_t getFACount(int32_t subnetIndex) { return this->subnetData[subnetIndex].FAIndex.size(); }
    ILCMap getFAIndex(int32_t subnetIndex, int32_t faIndex) {
        return this->subnetData[subnetIndex].FAIndex[faIndex];
    }
    int32_t getHMCount(int32_t subnetIndex) { return this->subnetData[subnetIndex].HMIndex.size(); }
    ILCMap getHMIndex(int32_t subnetIndex, int32_t hmIndex) {
        return this->subnetData[subnetIndex].HMIndex[hmIndex];
    }
    ILCMap getILCDataFromAddress(int32_t subnetIndex, uint8_t address) {
        return this->subnetData[subnetIndex].ILCDataFromAddress[address];
    }

    /**
     * Returns ILCMap for actuator with given ID.
     *
     * @param actuatorId ILC actuator ID.
     *
     * @return ILCMap object with Address and Subnet set to 255 when actuator
     * isn't found.
     */
    ILCMap getMap(int32_t actuatorId);

    void disableFA(int32_t actuatorId);
    void enableFA(int32_t actuatorId);
    void enableAllFA();

private:
    ForceActuatorApplicationSettings* _forceActuatorApplicationSettings;
};

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */

#endif /* ILCSUBNETDATA_H_ */
