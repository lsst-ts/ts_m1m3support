#ifndef TURNLIGHTSONCOMMAND_H_
#define TURNLIGHTSONCOMMAND_H_

#include <Command.h>
#include <SAL_MTM1M3C.h>
#include <DataTypes.h>

namespace LSST {
namespace M1M3 {
namespace SS {

class TurnLightsOnCommand : public Command {
private:
    Context* context;
    M1M3SSPublisher* publisher;

public:
    TurnLightsOnCommand(Context* context, M1M3SSPublisher* publisher, int32_t commandID,
                        MTM1M3_command_turnLightsOnC*);

    void execute() override;
    void ackInProgress() override;
    void ackComplete() override;
    void ackFailed(std::string reason) override;
};

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */

#endif /* TURNLIGHTSONCOMMAND_H_ */
