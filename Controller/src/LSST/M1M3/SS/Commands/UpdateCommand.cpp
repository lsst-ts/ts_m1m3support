/*
 * UpdateCommand.cpp
 *
 *  Created on: Sep 28, 2017
 *      Author: ccontaxis
 */

#include <UpdateCommand.h>
#include <Context.h>

namespace LSST {
namespace M1M3 {
namespace SS {

UpdateCommand::UpdateCommand(Context* context, pthread_mutex_t* updateMutex) {
    this->context = context;
    this->updateMutex = updateMutex;

    // Lock the update mutex to prevent the outer loop clock
    // thread from executing.
    pthread_mutex_lock(this->updateMutex);
}

UpdateCommand::~UpdateCommand() {
    // Release the update mutex to allow the outer loop clock
    // thread to continue executing.
    pthread_mutex_unlock(this->updateMutex);
}

void UpdateCommand::execute() {
    this->context->update(this);
}

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */
