#include <SetADCScanRateBusList.h>
#include <ILCSubnetData.h>
#include <ILCMessageFactory.h>
#include <SAL_MTM1M3C.h>
#include <spdlog/spdlog.h>

namespace LSST {
namespace M1M3 {
namespace SS {

SetADCScanRateBusList::SetADCScanRateBusList(ILCSubnetData* subnetData, ILCMessageFactory* ilcMessageFactory,
                                             MTM1M3_logevent_forceActuatorInfoC* forceInfo,
                                             MTM1M3_logevent_hardpointActuatorInfoC* hardpointInfo)
        : BusList(subnetData, ilcMessageFactory) {
    spdlog::debug("SetADCScanRateBusList: SetADCScanRateBusList()");
    this->forceInfo = forceInfo;
    this->hardpointInfo = hardpointInfo;
    for (int subnetIndex = 0; subnetIndex < SUBNET_COUNT; subnetIndex++) {
        this->startSubnet(subnetIndex);
        for (int faIndex = 0; faIndex < this->subnetData->getFACount(subnetIndex); faIndex++) {
            uint8_t address = this->subnetData->getFAIndex(subnetIndex, faIndex).Address;
            int32_t dataIndex = this->subnetData->getFAIndex(subnetIndex, faIndex).DataIndex;
            bool disabled = this->subnetData->getFAIndex(subnetIndex, faIndex).Disabled;
            if (!disabled) {
                this->ilcMessageFactory->setADCScanRate(&this->buffer, address,
                                                        this->forceInfo->adcScanRate[dataIndex]);
                this->expectedFAResponses[dataIndex] = 1;
            }
        }
        for (int hpIndex = 0; hpIndex < this->subnetData->getHPCount(subnetIndex); hpIndex++) {
            uint8_t address = this->subnetData->getHPIndex(subnetIndex, hpIndex).Address;
            int32_t dataIndex = this->subnetData->getHPIndex(subnetIndex, hpIndex).DataIndex;
            bool disabled = this->subnetData->getHPIndex(subnetIndex, hpIndex).Disabled;
            if (!disabled) {
                this->ilcMessageFactory->setADCScanRate(&this->buffer, address,
                                                        this->hardpointInfo->adcScanRate[dataIndex]);
                this->expectedHPResponses[dataIndex] = 1;
            }
        }
        this->endSubnet();
    }
    this->buffer.setLength(this->buffer.getIndex());
}

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */
