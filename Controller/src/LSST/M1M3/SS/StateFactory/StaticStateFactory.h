/*
 * StaticStateFactory.h
 *
 *  Created on: Sep 27, 2017
 *      Author: ccontaxis
 */

#ifndef STATICSTATEFACTORY_H_
#define STATICSTATEFACTORY_H_

#include <StateFactory.h>
#include <OfflineState.h>
#include <StandbyState.h>
#include <DisabledState.h>
#include <EnabledState.h>
#include <ParkedEngineeringState.h>

namespace LSST {
namespace M1M3 {
namespace SS {

class StaticStateFactory: public StateFactory {
private:
	OfflineState offlineState;
	StandbyState standbyState;
	DisabledState disabledState;
	EnabledState enabledState;
	ParkedEngineeringState parkedEngineeringState;

public:
	virtual IState* create(States::Type state);
	virtual void destroy(IState* state);
};

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */

#endif /* STATICSTATEFACTORY_H_ */
