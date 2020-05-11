#include <Command.h>

namespace LSST {
namespace M1M3 {
namespace SS {

Command::~Command() { }

bool Command::validate() { return true; }
void Command::execute() { }
void Command::ackInProgress() { }
void Command::ackComplete() { }
void Command::ackFailed(std::string reason) { }

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */
