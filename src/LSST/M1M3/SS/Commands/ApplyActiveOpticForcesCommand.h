#ifndef APPLYACTIVEOPTICFORCESCOMMAND_H_
#define APPLYACTIVEOPTICFORCESCOMMAND_H_

#include <Command.h>
#include <SAL_MTM1M3C.h>
#include <DataTypes.h>

namespace LSST {
namespace M1M3 {
namespace SS {

class ApplyActiveOpticForcesCommand : public Command {
private:
    Context* context;
    M1M3SSPublisher* publisher;
    MTM1M3_command_applyActiveOpticForcesC data;

public:
    ApplyActiveOpticForcesCommand(Context* context, M1M3SSPublisher* publisher, int32_t commandID,
                                  MTM1M3_command_applyActiveOpticForcesC* data);

    MTM1M3_command_applyActiveOpticForcesC* getData() { return &this->data; }

    void execute() override;
    void ackInProgress() override;
    void ackComplete() override;
    void ackFailed(std::string reason) override;
};

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */

#endif /* APPLYACTIVEOPTICFORCESCOMMAND_H_ */
