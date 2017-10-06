/*
 * StateFactory.cpp
 *
 *  Created on: Sep 27, 2017
 *      Author: ccontaxis
 */

#include <StateFactory.h>

namespace LSST {
namespace M1M3 {
namespace SS {

IState* StateFactory::create(States::Type state) { return 0; }

void StateFactory::destroy(IState* state) { }

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */
