/*
 * StateFactory.h
 *
 *  Created on: Sep 27, 2017
 *      Author: ccontaxis
 */

#ifndef STATEFACTORY_H_
#define STATEFACTORY_H_

#include <IStateFactory.h>

namespace LSST {
namespace M1M3 {
namespace SS {

class StateFactory: public IStateFactory {
public:
	virtual IState* create(States::Type state);
	virtual void destroy(IState* state);
};

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */

#endif /* STATEFACTORY_H_ */
