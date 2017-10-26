/*
 * ISubscriber.h
 *
 *  Created on: Sep 27, 2017
 *      Author: ccontaxis
 */

#ifndef ISUBSCRIBER_H_
#define ISUBSCRIBER_H_

namespace LSST {
namespace M1M3 {
namespace SS {

class ICommand;

class ISubscriber {
public:
	virtual ~ISubscriber();

	virtual ICommand* tryAcceptCommandStart();
	virtual ICommand* tryAcceptCommandEnable();
	virtual ICommand* tryAcceptCommandDisable();
	virtual ICommand* tryAcceptCommandStandby();
	virtual ICommand* tryAcceptCommandShutdown();
	virtual ICommand* tryAcceptCommandTurnAirOn();
	virtual ICommand* tryAcceptCommandTurnAirOff();
	virtual ICommand* tryAcceptCommandApplyOffsetForces();
	virtual ICommand* tryAcceptCommandClearOffsetForces();
	virtual ICommand* tryAcceptCommandRaiseM1M3();
	virtual ICommand* tryAcceptCommandLowerM1M3();
	virtual ICommand* tryAcceptCommandApplyAberrationByBendingModes();
	virtual ICommand* tryAcceptCommandApplyAberrationByForces();
	virtual ICommand* tryAcceptCommandClearAberration();
	virtual ICommand* tryAcceptCommandApplyAOSCorrectionByBendingModes();
	virtual ICommand* tryAcceptCommandApplyAOSCorrectionByForces();
	virtual ICommand* tryAcceptCommandClearAOSCorrection();
};

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */

#endif /* ISUBSCRIBER_H_ */
