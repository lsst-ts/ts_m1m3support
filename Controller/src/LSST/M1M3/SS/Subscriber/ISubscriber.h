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
};

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */

#endif /* ISUBSCRIBER_H_ */
