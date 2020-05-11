#ifndef TESTHARDPOINTCOMMAND_H_
#define TESTHARDPOINTCOMMAND_H_

#include <Command.h>
#include <SAL_MTM1M3C.h>
#include <DataTypes.h>

namespace LSST {
namespace M1M3 {
namespace SS {

class TestHardpointCommand : public Command {
private:
    Context* context;
    M1M3SSPublisher* publisher;
    MTM1M3_command_testHardpointC data;

public:
    TestHardpointCommand(Context* context, M1M3SSPublisher* publisher, int32_t commandID,
                         MTM1M3_command_testHardpointC* data);

    MTM1M3_command_testHardpointC* getData() { return &this->data; }

    bool validate() override;
    void execute() override;
    void ackInProgress() override;
    void ackComplete() override;
    void ackFailed(std::string reason) override;
};

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */

#endif /* TESTHARDPOINTCOMMAND_H_ */
