/*
 * EngineeringState.cpp
 *
 *  Created on: Nov 3, 2017
 *      Author: ccontaxis
 */

#include <EngineeringState.h>

namespace LSST {
namespace M1M3 {
namespace SS {

EngineeringState::EngineeringState(M1M3SSPublisher* publisher) : EnabledState(publisher, "EngineeringState") { }
EngineeringState::EngineeringState(M1M3SSPublisher* publisher, std::string name) : EnabledState(publisher, name) { }

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */
