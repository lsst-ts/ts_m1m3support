/*
 * ExpansionFPGA.h
 *
 *  Created on: Jan 9, 2018
 *      Author: ccontaxis
 */

#ifndef EXPANSIONFPGA_H_
#define EXPANSIONFPGA_H_

#include <IExpansionFPGA.h>

namespace LSST {
namespace M1M3 {
namespace SS {

class ExpansionFPGAApplicationSettings;

class ExpansionFPGA : public IExpansionFPGA {
   private:
    ExpansionFPGAApplicationSettings* expansionFPGAApplicationSettings;
    uint32_t session;
    size_t remaining;

   public:
    ExpansionFPGA(ExpansionFPGAApplicationSettings* expansionFPGAApplicationSettings);

    int32_t initialize();
    int32_t open();
    int32_t close();
    int32_t finalize();

    bool isErrorCode(int32_t status);

    int32_t sample();

    int32_t readSlot1(float* data);
    int32_t readSlot2(uint32_t* data);
};

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */

#endif /* EXPANSIONFPGA_H_ */
