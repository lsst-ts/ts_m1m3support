#include <Controller.h>
#include <Context.h>
#include <CommandFactory.h>
#include <Command.h>
#include <spdlog/spdlog.h>

namespace LSST {
namespace M1M3 {
namespace SS {

Controller::Controller(CommandFactory* commandFactory) {
    spdlog::debug("Controller: Controller()");
    this->commandFactory = commandFactory;
    pthread_mutex_init(&this->mutex, NULL);
}

Controller::~Controller() {
    this->clear();
    pthread_mutex_destroy(&this->mutex);
}

void Controller::lock() {
    spdlog::trace("Controller: lock()");
    pthread_mutex_lock(&this->mutex);
}

void Controller::unlock() {
    spdlog::trace("Controller: unlock()");
    pthread_mutex_unlock(&this->mutex);
}

void Controller::clear() {
    spdlog::trace("Controller: clear()");
    this->lock();
    Command* command;
    while (!this->queue.empty()) {
        command = this->dequeue();
        this->commandFactory->destroy(command);
    }
    this->unlock();
}

void Controller::enqueue(Command* command) {
    spdlog::trace("Controller: enqueue()");
    this->queue.push(command);
}

Command* Controller::dequeue() {
    spdlog::trace("Controller: dequeue()");
    if (!this->queue.empty()) {
        Command* command = this->queue.front();
        this->queue.pop();
        return command;
    }
    return 0;
}

void Controller::execute(Command* command) {
    spdlog::trace("Controller: execute()");
    try {
        command->ackInProgress();
        command->execute();
        command->ackComplete();
    } catch (std::exception& e) {
        spdlog::error("Cannot execute command: {}", e.what());
        command->ackFailed(e.what());
    }

    this->commandFactory->destroy(command);
}

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */
