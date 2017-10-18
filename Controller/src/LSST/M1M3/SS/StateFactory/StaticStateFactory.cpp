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

IState* StaticStateFactory::create(States::Type state) {
	switch(state) {
	case States::OfflineState: return &this->offlineState;
	case States::StandbyState: return &this->standbyState;
	case States::DisabledState: return &this->disabledState;
	case States::EnabledState: return &this->enabledState;
	case States::ParkedEngineeringState: return &this->parkedEngineeringState;
	default: return 0;
	}
}

void StaticStateFactory::destroy(IState* state) { }

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */
