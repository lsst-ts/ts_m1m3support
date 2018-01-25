/*
 * StaticStateFactory.cpp
 *
 *  Created on: Sep 27, 2017
 *      Author: ccontaxis
 */

#include <StaticStateFactory.h>

namespace LSST {
namespace M1M3 {
namespace SS {

StaticStateFactory::StaticStateFactory(M1M3SSPublisher* publisher) :
		offlineState(publisher),
		standbyState(publisher),
		disabledState(publisher),
		enabledState(publisher),
		parkedState(publisher),
		raisingState(publisher),
		activeState(publisher),
		loweringState(publisher),
		engineeringState(publisher),
		parkedEngineeringState(publisher),
		raisingEngineeringState(publisher),
		activeEngineeringState(publisher),
		loweringEngineeringState(publisher),
		faultState(publisher) { }

State* StaticStateFactory::create(States::Type state) {
	switch(state) {
	case States::OfflineState: return &this->offlineState;
	case States::StandbyState: return &this->standbyState;
	case States::DisabledState: return &this->disabledState;
	case States::EnabledState: return &this->enabledState;
	case States::ParkedState: return &this->parkedState;
	case States::RaisingState: return &this->raisingState;
	case States::ActiveState: return &this->activeState;
	case States::LoweringState: return &this->loweringState;
	case States::EngineeringState: return &this->engineeringState;
	case States::ParkedEngineeringState: return &this->parkedEngineeringState;
	case States::RaisingEngineeringState: return &this->raisingEngineeringState;
	case States::ActiveEngineeringState: return &this->activeEngineeringState;
	case States::LoweringEngineeringState: return &this->loweringEngineeringState;
	case States::FaultState: return &this->faultState;
	default: return 0;
	}
}

void StaticStateFactory::destroy(State* state) { }

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */
