#include <TestAirCommand.h>
#include <Context.h>
#include <M1M3SSPublisher.h>

namespace LSST {
namespace M1M3 {
namespace SS {

TestAirCommand::TestAirCommand(Context* context, M1M3SSPublisher* publisher, int32_t commandID,
                               MTM1M3_command_testAirC*) {
    this->context = context;
    this->publisher = publisher;
    this->commandID = commandID;
}

void TestAirCommand::execute() { this->context->testAir(this); }

void TestAirCommand::ackInProgress() {
    this->publisher->ackCommandtestAir(this->commandID, ACK_INPROGRESS, "In-Progress");
}

void TestAirCommand::ackComplete() {
    this->publisher->ackCommandtestAir(this->commandID, ACK_COMPLETE, "Completed");
}

void TestAirCommand::ackFailed(std::string reason) {
    this->publisher->ackCommandtestAir(this->commandID, ACK_FAILED, "Failed: " + reason);
}

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */
