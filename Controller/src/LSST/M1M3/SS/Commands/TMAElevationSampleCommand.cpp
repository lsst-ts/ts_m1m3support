/*
 * TMAElevationSampleCommand.cpp
 *
 *  Created on: Nov 2, 2017
 *      Author: ccontaxis
 */

#include <TMAElevationSampleCommand.h>
#include <Context.h>
#include <cstring>

namespace LSST {
namespace M1M3 {
namespace SS {

TMAElevationSampleCommand::TMAElevationSampleCommand(Context* context, MTMount_ElevationC* data) {
    this->context = context;
    memcpy(&this->data, data, sizeof(MTMount_ElevationC));
}

void TMAElevationSampleCommand::execute() {
    this->context->storeTMAElevationSample(this);
}

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */
