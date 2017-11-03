/*
 * TMAAzimuthSampleCommand.cpp
 *
 *  Created on: Nov 2, 2017
 *      Author: ccontaxis
 */

#include <TMAAzimuthSampleCommand.h>
#include <IContext.h>
#include <cstring>

namespace LSST {
namespace M1M3 {
namespace SS {

TMAAzimuthSampleCommand::TMAAzimuthSampleCommand(IContext* context, MTMount_AzC* data) {
	this->context = context;
	//memcpy(this->data, data, sizeof(MTMount_AzC));
}

void TMAAzimuthSampleCommand::execute() {
	this->context->storeTMAAzimuthSample(this);
}

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */
