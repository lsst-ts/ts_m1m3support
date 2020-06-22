#include <ControllerThread.h>
#include <Controller.h>
#include <unistd.h>
#include <spdlog/spdlog.h>

namespace LSST {
namespace M1M3 {
namespace SS {

ControllerThread::ControllerThread(Controller* controller) {
    this->controller = controller;
    this->keepRunning = true;
}

void ControllerThread::run() {
    spdlog::info("ControllerThread: Start");
    while (this->keepRunning) {
        this->controller->lock();
        Command* command = this->controller->dequeue();
        this->controller->unlock();
        if (command) {
            this->controller->execute(command);
        } else {
            usleep(100);
        }
    }
    spdlog::info("ControllerThread: Completed");
}

void ControllerThread::stop() { this->keepRunning = false; }

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */
