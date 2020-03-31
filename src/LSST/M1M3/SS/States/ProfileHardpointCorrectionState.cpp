#include <ProfileHardpointCorrectionState.h>
#include <Model.h>
#include <ForceController.h>
#include <ProfileController.h>
#include <MirrorForceProfile.h>
#include <SafetyController.h>
#include <spdlog/spdlog.h>

namespace LSST {
namespace M1M3 {
namespace SS {

ProfileHardpointCorrectionState::ProfileHardpointCorrectionState(M1M3SSPublisher* publisher)
        : EnabledState(publisher, "LoweringFaultState") {}

States::Type ProfileHardpointCorrectionState::update(UpdateCommand* command, Model* model) {
    spdlog::trace("ProfileHardpointCorrectionState: update()");
    MirrorForceProfileRecord force = model->getProfileController()->getMirrorForceProfileData();
    model->getForceController()->applyOffsetForcesByMirrorForces(force.XForce, force.YForce, force.ZForce,
                                                                 force.XMoment, force.YMoment, force.ZMoment);
    EnabledState::update(command, model);
    if (model->getProfileController()->incMirrorForceProfile()) {
        return model->getSafetyController()->checkSafety(States::ActiveEngineeringState);
    }
    return model->getSafetyController()->checkSafety(States::NoStateTransition);
}

States::Type ProfileHardpointCorrectionState::abortProfile(AbortProfileCommand* command, Model* model) {
    spdlog::info("ProfileHardpointCorrectionState: abortProfile()");
    model->getForceController()->zeroOffsetForces();
    return model->getSafetyController()->checkSafety(States::ActiveEngineeringState);
}

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */
