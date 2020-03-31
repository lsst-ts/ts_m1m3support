#include <OuterLoopClockThread.h>
#include <CommandFactory.h>
#include <Controller.h>
#include <FPGA.h>
#include <FPGAAddresses.h>
#include <Command.h>
#include <M1M3SSPublisher.h>
#include <Timestamp.h>
#include <spdlog/spdlog.h>

namespace LSST {
namespace M1M3 {
namespace SS {

OuterLoopClockThread::OuterLoopClockThread(CommandFactory* commandFactory, Controller* controller,
                                           M1M3SSPublisher* publisher) {
    this->commandFactory = commandFactory;
    this->controller = controller;
    this->publisher = publisher;
    this->keepRunning = true;
    pthread_mutex_init(&this->updateMutex, NULL);
}

OuterLoopClockThread::~OuterLoopClockThread() { pthread_mutex_destroy(&this->updateMutex); }

void OuterLoopClockThread::run() {
    spdlog::info("OuterLoopClockThread: Start");
    while (this->keepRunning) {
        if (IFPGA::get().waitForOuterLoopClock(1000) == 0) {
            this->controller->lock();
            if (this->keepRunning) {
                this->controller->enqueue(
                        this->commandFactory->create(Commands::UpdateCommand, &this->updateMutex));
            }
            this->controller->unlock();
            pthread_mutex_lock(&this->updateMutex);
            pthread_mutex_unlock(&this->updateMutex);
            IFPGA::get().ackOuterLoopClock();
        } else {
            spdlog::warn("OuterLoopClockThread: Failed to receive outer loop clock");
        }
    }
    spdlog::info("OuterLoopClockThread: Completed");
}

void OuterLoopClockThread::stop() { this->keepRunning = false; }

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */
