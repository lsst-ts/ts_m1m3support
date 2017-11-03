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
#include <ParkedState.h>
#include <RaisingState.h>
#include <ActiveState.h>
#include <LoweringState.h>
#include <EngineeringState.h>
#include <ParkedEngineeringState.h>
#include <RaisingEngineeringState.h>
#include <ActiveEngineeringState.h>
#include <LoweringEngineeringState.h>
#include <FaultState.h>

namespace LSST {
namespace M1M3 {
namespace SS {

class StaticStateFactory: public StateFactory {
private:
	OfflineState offlineState;
	StandbyState standbyState;
	DisabledState disabledState;
	EnabledState enabledState;
	ParkedState parkedState;
	RaisingState raisingState;
	ActiveState activeState;
	LoweringState loweringState;
	EngineeringState engineeringState;
	ParkedEngineeringState parkedEngineeringState;
	RaisingEngineeringState raisingEngineeringState;
	ActiveEngineeringState activeEngineeringState;
	LoweringEngineeringState loweringEngineeringState;
	FaultState faultState;

public:
	IState* create(States::Type state);
	void destroy(IState* state);
};

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */

#endif /* STATICSTATEFACTORY_H_ */
