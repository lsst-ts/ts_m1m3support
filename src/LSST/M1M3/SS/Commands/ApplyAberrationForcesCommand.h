#ifndef APPLYABERRATIONFORCESCOMMAND_H_
#define APPLYABERRATIONFORCESCOMMAND_H_

#include <Command.h>
#include <SAL_MTM1M3C.h>
#include <DataTypes.h>

namespace LSST {
namespace M1M3 {
namespace SS {

class ApplyAberrationForcesCommand : public Command {
private:
    Context* context;
    M1M3SSPublisher* publisher;
    MTM1M3_command_applyAberrationForcesC data;

public:
    ApplyAberrationForcesCommand(Context* context, M1M3SSPublisher* publisher, int32_t commandID,
                                 MTM1M3_command_applyAberrationForcesC* data);

    MTM1M3_command_applyAberrationForcesC* getData() { return &this->data; }

    void execute() override;
    void ackInProgress() override;
    void ackComplete() override;
    void ackFailed(std::string reason) override;
};

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */

#endif /* APPLYABERRATIONFORCESCOMMAND_H_ */
