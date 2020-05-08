#ifndef STARTCOMMAND_H_
#define STARTCOMMAND_H_

#include <Command.h>
#include <SAL_MTM1M3C.h>
#include <DataTypes.h>

namespace LSST {
namespace M1M3 {
namespace SS {

/*!
 * This command is responsible for transitioning the state
 * machine from the standby state to the disabled state.
 * This is an external command and can be issued via SAL.
 */
class StartCommand : public Command {
private:
    Context* context;
    M1M3SSPublisher* publisher;
    MTM1M3_command_startC data;

public:
    StartCommand(Context* context, M1M3SSPublisher* publisher, int32_t commandID,
                 MTM1M3_command_startC* data);

    MTM1M3_command_startC* getData() { return &this->data; }

    bool validate();
    void execute();
    void ackInProgress();
    void ackComplete();
    void ackFailed(std::string reason);
};

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */

#endif /* STARTCOMMAND_H_ */
