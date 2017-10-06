/*
 * IStateFactory.h
 *
 *  Created on: Sep 27, 2017
 *      Author: ccontaxis
 */

#ifndef ISTATEFACTORY_H_
#define ISTATEFACTORY_H_

#include <StateTypes.h>

namespace LSST {
namespace M1M3 {
namespace SS {

class IState;

class IStateFactory {
public:
	virtual ~IStateFactory();

	virtual IState* create(States::Type state);
	virtual void destroy(IState* state);
};

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */

#endif /* ISTATEFACTORY_H_ */
