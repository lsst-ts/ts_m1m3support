#ifndef EXITENGINEERINGCOMMAND_H_
#define EXITENGINEERINGCOMMAND_H_

#include <Command.h>
#include <SAL_MTM1M3C.h>
#include <DataTypes.h>

namespace LSST {
namespace M1M3 {
namespace SS {

class ExitEngineeringCommand : public Command {
private:
    Context* context;
    M1M3SSPublisher* publisher;

public:
    ExitEngineeringCommand(Context* context, M1M3SSPublisher* publisher, int32_t commandID,
                           MTM1M3_command_exitEngineeringC*);

    void execute() override;
    void ackInProgress() override;
    void ackComplete() override;
    void ackFailed(std::string reason) override;
};

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */

#endif /* EXITENGINEERINGCOMMAND_H_ */
