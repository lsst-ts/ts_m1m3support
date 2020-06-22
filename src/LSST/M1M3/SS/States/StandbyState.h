#ifndef STANDBYSTATE_H_
#define STANDBYSTATE_H_

#include <State.h>

namespace LSST {
namespace M1M3 {
namespace SS {

class StandbyState : public State {
public:
    StandbyState(M1M3SSPublisher* publisher);

    virtual States::Type update(UpdateCommand* command, Model* model);
    virtual States::Type start(StartCommand* command, Model* model);
    virtual States::Type shutdown(ShutdownCommand* command, Model* model);
};

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */

#endif /* STANDBYSTATE_H_ */
