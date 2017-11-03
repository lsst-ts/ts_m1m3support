/*
 * TMAElevationSampleCommand.h
 *
 *  Created on: Nov 2, 2017
 *      Author: ccontaxis
 */

#ifndef TMAELEVATIONSAMPLECOMMAND_H_
#define TMAELEVATIONSAMPLECOMMAND_H_

#include <Command.h>
//#include <SAL_MTMountC.h>

struct MTMount_AltC;

namespace LSST {
namespace M1M3 {
namespace SS {

class TMAElevationSampleCommand: public Command {
private:
	IContext* context;

	MTMount_AltC* data;

public:
	TMAElevationSampleCommand(IContext* context, MTMount_AltC* data);

	MTMount_AltC* getData() { return this->data; }

	void execute();
};

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */

#endif /* TMAELEVATIONSAMPLECOMMAND_H_ */
