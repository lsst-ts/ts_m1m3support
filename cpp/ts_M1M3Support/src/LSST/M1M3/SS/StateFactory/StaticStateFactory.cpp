/*
 * StaticStateFactory.cpp
 *
 *  Created on: Sep 27, 2017
 *      Author: ccontaxis
 */

#include <StaticStateFactory.h>
#include <OfflineState.h>
#include <StandbyState.h>
#include <DisabledState.h>
#include <EnabledState.h>

namespace LSST {
namespace M1M3 {
namespace SS {

IState* StaticStateFactory::create(States::Type state) {
	switch(state) {
	case States::OfflineState: return &this->offlineState;
	case States::StandbyState: return &this->standbyState;
	case States::DisabledState: return &this->disabledState;
	case States::EnabledState: return &this->enabledState;
	default: return 0;
	}
}

void StaticStateFactory::destroy(IState* state) { }

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */
