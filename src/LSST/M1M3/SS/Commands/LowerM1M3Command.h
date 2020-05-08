#ifndef LOWERM1M3COMMAND_H_
#define LOWERM1M3COMMAND_H_

#include <Command.h>
#include <SAL_MTM1M3C.h>
#include <DataTypes.h>

namespace LSST {
namespace M1M3 {
namespace SS {

class LowerM1M3Command : public Command {
private:
    Context* context;
    M1M3SSPublisher* publisher;

public:
    LowerM1M3Command(Context* context, M1M3SSPublisher* publisher, int32_t commandID,
                     MTM1M3_command_lowerM1M3C*);

    void execute() override;
    void ackInProgress() override;
    void ackComplete() override;
    void ackFailed(std::string reason) override;
};

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */

#endif /* LOWERM1M3COMMAND_H_ */
