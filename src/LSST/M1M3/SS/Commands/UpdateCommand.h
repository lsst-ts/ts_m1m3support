#ifndef UPDATECOMMAND_H_
#define UPDATECOMMAND_H_

#include <Command.h>
#include <pthread.h>

namespace LSST {
namespace M1M3 {
namespace SS {

/*!
 * This command is responsible for running a periodic update.
 * This is an internal command only and cannot be issued via SAL.
 * The command is generated by the FPGA's outer loop clock.
 */
class UpdateCommand : public Command {
private:
    Context* context;
    pthread_mutex_t* updateMutex;

public:
    UpdateCommand(Context* context, pthread_mutex_t* updateMutext);
    ~UpdateCommand();

    void execute() override;
};

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */

#endif /* UPDATECOMMAND_H_ */
