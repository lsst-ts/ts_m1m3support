#ifndef ENABLECOMMAND_H_
#define ENABLECOMMAND_H_

#include <Command.h>
#include <SAL_MTM1M3C.h>
#include <DataTypes.h>

namespace LSST {
namespace M1M3 {
namespace SS {

/*!
 * This command is responsible for transitioning the state
 * machine from the disabled state to the enabled state.
 * This is an external command and can be issued via SAL.
 */
class EnableCommand : public Command {
private:
    Context* context;
    M1M3SSPublisher* publisher;
    MTM1M3_command_enableC data;

public:
    EnableCommand(Context* context, M1M3SSPublisher* publisher, int32_t commandID,
                  MTM1M3_command_enableC* data);

    MTM1M3_command_enableC* getData() { return &this->data; }

    void execute();
    void ackInProgress();
    void ackComplete();
    void ackFailed(std::string reason);
};

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */

#endif /* ENABLECOMMAND_H_ */
