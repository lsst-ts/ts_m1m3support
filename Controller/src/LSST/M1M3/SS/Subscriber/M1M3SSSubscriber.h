/*
 * M1M3SSSubscriber.h
 *
 *  Created on: Sep 27, 2017
 *      Author: ccontaxis
 */

#ifndef M1M3SSSUBSCRIBER_H_
#define M1M3SSSUBSCRIBER_H_

#include <ISubscriber.h>
#include <SAL_m1m3C.h>

class SAL_m1m3;

namespace LSST {
namespace M1M3 {
namespace SS {

class ICommandFactory;

class M1M3SSSubscriber: public ISubscriber {
private:
	SAL_m1m3* m1m3SAL;
	ICommandFactory* commandFactory;
	m1m3_command_StartC startData;
	m1m3_command_EnableC enableData;
	m1m3_command_DisableC disableData;
	m1m3_command_StandbyC standbyData;
	m1m3_command_ShutdownC shutdownData;

public:
	M1M3SSSubscriber(SAL_m1m3* m1m3SAL, ICommandFactory* commandFactory);

	virtual ICommand* tryAcceptCommandStart();
	virtual ICommand* tryAcceptCommandEnable();
	virtual ICommand* tryAcceptCommandDisable();
	virtual ICommand* tryAcceptCommandStandby();
	virtual ICommand* tryAcceptCommandShutdown();
};

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */

#endif /* M1M3SSSUBSCRIBER_H_ */
