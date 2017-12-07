/*
 * ICommandFactory.cpp
 *
 *  Created on: Sep 29, 2017
 *      Author: ccontaxis
 */

#include <ICommandFactory.h>

namespace LSST {
namespace M1M3 {
namespace SS {

ICommandFactory::~ICommandFactory() { }

ICommand* ICommandFactory::create(Commands::Type commandType, void* data, int32_t commandID) { return 0; }
void ICommandFactory::destroy(ICommand* command) { }

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */
