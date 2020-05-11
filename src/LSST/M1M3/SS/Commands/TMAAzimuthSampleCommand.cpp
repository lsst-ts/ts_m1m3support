#include <TMAAzimuthSampleCommand.h>
#include <Context.h>
#include <cstring>

namespace LSST {
namespace M1M3 {
namespace SS {

TMAAzimuthSampleCommand::TMAAzimuthSampleCommand(Context* context, MTMount_AzimuthC* data) {
    this->context = context;
    this->commandID = -1;
    memcpy(&this->data, data, sizeof(MTMount_AzimuthC));
}

void TMAAzimuthSampleCommand::execute() { this->context->storeTMAAzimuthSample(this); }

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */
