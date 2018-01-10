/*
 * IStateFactory.cpp
 *
 *  Created on: Sep 27, 2017
 *      Author: ccontaxis
 */

#include <IStateFactory.h>

namespace LSST {
namespace M1M3 {
namespace SS {

IStateFactory::~IStateFactory() { }

IState* IStateFactory::create(States::Type state) { return 0; }
void IStateFactory::destroy(IState* state) { }

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */
