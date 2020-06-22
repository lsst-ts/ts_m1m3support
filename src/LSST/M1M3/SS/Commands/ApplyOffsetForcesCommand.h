#ifndef APPLYOFFSETFORCESCOMMAND_H_
#define APPLYOFFSETFORCESCOMMAND_H_

#include <Command.h>
#include <SAL_MTM1M3C.h>
#include <DataTypes.h>

namespace LSST {
namespace M1M3 {
namespace SS {

class ApplyOffsetForcesCommand : public Command {
private:
    Context* context;
    M1M3SSPublisher* publisher;
    MTM1M3_command_applyOffsetForcesC data;

public:
    ApplyOffsetForcesCommand(Context* context, M1M3SSPublisher* publisher, int32_t commandID,
                             MTM1M3_command_applyOffsetForcesC* data);

    MTM1M3_command_applyOffsetForcesC* getData() { return &this->data; }

    bool validate();
    void execute();
    void ackInProgress();
    void ackComplete();
    void ackFailed(std::string reason);
};

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */

#endif /* APPLYOFFSETFORCESCOMMAND_H_ */
