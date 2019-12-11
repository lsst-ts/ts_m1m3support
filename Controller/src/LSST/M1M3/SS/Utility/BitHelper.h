/*
 * BitHelper.h
 *
 *  Created on: Nov 19, 2018
 *      Author: ccontaxis
 */

#ifndef LSST_M1M3_SS_UTILITY_BITHELPER_H_
#define LSST_M1M3_SS_UTILITY_BITHELPER_H_

#include <DataTypes.h>

namespace LSST {
namespace M1M3 {
namespace SS {

class BitHelper {
   public:
    BitHelper();

    static inline void set(unsigned long long* data, int32_t bit, bool value) {
        (*data) = ((*data) & ~(1 << bit)) | (value ? (1 << bit) : 0);
    }

    static inline bool get(unsigned long long data, int32_t bit) {
        return data & (1 << bit);
    }
};

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */

#endif /* LSST_M1M3_SS_UTILITY_BITHELPER_H_ */
