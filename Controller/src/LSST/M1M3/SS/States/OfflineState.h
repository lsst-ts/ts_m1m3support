/*
 * OfflineState.h
 *
 *  Created on: Sep 26, 2017
 *      Author: ccontaxis
 */

#ifndef OFFLINESTATE_H_
#define OFFLINESTATE_H_

#include <State.h>

namespace LSST {
namespace M1M3 {
namespace SS {

class OfflineState : public State {
   public:
    OfflineState(M1M3SSPublisher* publisher);

    virtual States::Type boot(BootCommand* command, Model* model);
};

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */

#endif /* OFFLINESTATE_H_ */
