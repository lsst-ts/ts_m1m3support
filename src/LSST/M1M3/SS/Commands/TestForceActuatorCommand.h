#ifndef TESTFORCEACTUATORCOMMAND_H_
#define TESTFORCEACTUATORCOMMAND_H_

#include <Command.h>
#include <SAL_MTM1M3C.h>
#include <DataTypes.h>

namespace LSST {
namespace M1M3 {
namespace SS {

class TestForceActuatorCommand : public Command {
private:
    Context* context;
    M1M3SSPublisher* publisher;
    MTM1M3_command_testForceActuatorC data;

public:
    TestForceActuatorCommand(Context* context, M1M3SSPublisher* publisher, int32_t commandID,
                             MTM1M3_command_testForceActuatorC* data);

    bool validate() override;
    void execute() override;
    void ackInProgress() override;
    void ackComplete() override;
    void ackFailed(std::string reason) override;
};

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */

#endif /* TESTFORCEACTUATORCOMMAND_H_ */
