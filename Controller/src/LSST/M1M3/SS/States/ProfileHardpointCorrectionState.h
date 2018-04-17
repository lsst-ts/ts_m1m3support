/*
 * ProfileHardpointCorrectionState.h
 *
 *  Created on: Feb 9, 2018
 *      Author: ccontaxis
 */

#ifndef PROFILEHARDPOINTCORRECTIONSTATE_H_
#define PROFILEHARDPOINTCORRECTIONSTATE_H_

#include <EnabledState.h>

namespace LSST {
namespace M1M3 {
namespace SS {

class M1M3SSPublisher;
class UpdateCommand;
class Model;

class ProfileHardpointCorrectionState: public EnabledState {
public:
	ProfileHardpointCorrectionState(M1M3SSPublisher* publisher);

	States::Type update(UpdateCommand* command, Model* model);
	States::Type abortProfile(AbortProfileCommand* command, Model* model);
};

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */

#endif /* PROFILEHARDPOINTCORRECTIONSTATE_H_ */
