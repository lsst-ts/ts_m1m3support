#ifndef RUNMIRRORFORCEPROFILECOMMAND_H_
#define RUNMIRRORFORCEPROFILECOMMAND_H_

#include <Command.h>
#include <SAL_MTM1M3C.h>
#include <DataTypes.h>

namespace LSST {
namespace M1M3 {
namespace SS {

class RunMirrorForceProfileCommand : public Command {
private:
    Context* context;
    M1M3SSPublisher* publisher;
    MTM1M3_command_runMirrorForceProfileC data;

public:
    RunMirrorForceProfileCommand(Context* context, M1M3SSPublisher* publisher, int32_t commandID,
                                 MTM1M3_command_runMirrorForceProfileC* data);

    MTM1M3_command_runMirrorForceProfileC* getData() { return &this->data; }

    bool validate();
    void execute();
    void ackInProgress();
    void ackComplete();
    void ackFailed(std::string reason);
};

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */

#endif /* RUNMIRRORFORCEPROFILECOMMAND_H_ */
