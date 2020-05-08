#ifndef RAISEM1M3COMMAND_H_
#define RAISEM1M3COMMAND_H_

#include <Command.h>
#include <SAL_MTM1M3C.h>
#include <DataTypes.h>

namespace LSST {
namespace M1M3 {
namespace SS {

class RaiseM1M3Command : public Command {
private:
    Context* context;
    M1M3SSPublisher* publisher;
    MTM1M3_command_raiseM1M3C data;

public:
    RaiseM1M3Command(Context* context, M1M3SSPublisher* publisher, int32_t commandID,
                     MTM1M3_command_raiseM1M3C* data);

    MTM1M3_command_raiseM1M3C* getData() { return &this->data; }

    bool validate() override;
    void execute() override;
    void ackInProgress() override;
    void ackComplete() override;
    void ackFailed(std::string reason) override;
};

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */

#endif /* RAISEM1M3COMMAND_H_ */
