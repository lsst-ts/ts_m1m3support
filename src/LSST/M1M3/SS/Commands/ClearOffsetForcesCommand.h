#ifndef CLEAROFFSETFORCESCOMMAND_H_
#define CLEAROFFSETFORCESCOMMAND_H_

#include <Command.h>
#include <SAL_MTM1M3C.h>
#include <DataTypes.h>

namespace LSST {
namespace M1M3 {
namespace SS {

class ClearOffsetForcesCommand : public Command {
private:
    Context* context;
    M1M3SSPublisher* publisher;

public:
    ClearOffsetForcesCommand(Context* context, M1M3SSPublisher* publisher, int32_t commandID,
                             MTM1M3_command_clearOffsetForcesC*);

    void execute() override;
    void ackInProgress() override;
    void ackComplete() override;
    void ackFailed(std::string reason) override;
};

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */

#endif /* CLEAROFFSETFORCESCOMMAND_H_ */
