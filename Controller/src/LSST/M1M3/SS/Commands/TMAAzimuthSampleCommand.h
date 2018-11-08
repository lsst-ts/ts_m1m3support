/*
 * TMAAzimuthSampleCommand.h
 *
 *  Created on: Nov 2, 2017
 *      Author: ccontaxis
 */

#ifndef TMAAZIMUTHSAMPLECOMMAND_H_
#define TMAAZIMUTHSAMPLECOMMAND_H_

#include <Command.h>
#include <SAL_MTMountC.h>

struct MTMount_AzimuthC;

namespace LSST {
namespace M1M3 {
namespace SS {

class TMAAzimuthSampleCommand: public Command {
private:
	Context* context;

	MTMount_AzimuthC data;

public:
	TMAAzimuthSampleCommand(Context* context, MTMount_AzimuthC* data);

	MTMount_AzimuthC* getData() { return &this->data; }

	void execute();
};

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */

#endif /* TMAAZIMUTHSAMPLECOMMAND_H_ */
