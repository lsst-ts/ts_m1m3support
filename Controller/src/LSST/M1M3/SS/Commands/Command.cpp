/*
 * Command.cpp
 *
 *  Created on: Sep 26, 2017
 *      Author: ccontaxis
 */

#include <Command.h>

namespace LSST {
namespace M1M3 {
namespace SS {

Command::~Command() { }

bool Command::validate() { return true; }
void Command::execute() { }
void Command::ackInProgress() { this->ack(301, 0, "In-Progress"); }
void Command::ackComplete() { this->ack(303, 0, "Complete"); }
void Command::ackNotPermitted(int32_t errorCode, std::string reason) { this->ack(-300, errorCode, "Not Permitted: " + reason); }
void Command::ackAborted(int32_t errorCode, std::string reason) { this->ack(-303, errorCode, "Aborted: " + reason); }
void Command::ackFailed(int32_t errorCode, std::string reason) { this->ack(-302, errorCode, "Failed: " + reason); }
void Command::ackInvalidState(std::string reason) { this->ackFailed(-320, reason); }
void Command::ackInvalidParameter(std::string reason) { this->ackFailed(-321, reason); }
void Command::ackAlreadyInProgress(std::string reason) { this->ackFailed(-322, reason); }
void Command::ackExecutionBlocked(std::string reason) { this->ackFailed(-323, reason); }
void Command::ackAlreadyInState(std::string reason) { this->ackFailed(-324, reason); }
void Command::ack(int32_t ackCode, int32_t errorCode, std::string description) { }

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */
