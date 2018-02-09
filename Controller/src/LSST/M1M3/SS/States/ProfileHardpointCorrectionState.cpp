/*
 * ProfileHardpointCorrectionState.cpp
 *
 *  Created on: Feb 9, 2018
 *      Author: ccontaxis
 */

#include <ProfileHardpointCorrectionState.h>
#include <Model.h>
#include <ForceController.h>
#include <ProfileController.h>
#include <HardpointCorrectionProfileForce.h>

namespace LSST {
namespace M1M3 {
namespace SS {

ProfileHardpointCorrectionState::ProfileHardpointCorrectionState(M1M3SSPublisher* publisher) : FaultState(publisher, "LoweringFaultState") { }

States::Type ProfileHardpointCorrectionState::update(UpdateCommand* command, Model* model) {
	HardpointCorrectionProfileForce force = model->getProfileController()->getHardpointCorrectionProfileData();
	model->getForceController()->applyOffsetForces(force.XForce, force.YForce, force.ZForce, force.XMoment, force.YMoment, force.ZMoment);
	EnabledState::update(command, model);
	if (model->getProfileController()->incHardpointCorrectionProfile()) {
		return model->getSafetyController()->checkSafety(States::ActiveEngineeringState);
	}
	return model->getSafetyController()->checkSafety(States::NoStateTransition);
}

States::Type ProfileHardpointCorrectionState::abortProfile(AbortProfileCommand* command, Model* model) {
	model->getForceController()->zeroOffsetForces();
	return model->getSafetyController()->checkSafety(States::ActiveEngineeringState);
}

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */
