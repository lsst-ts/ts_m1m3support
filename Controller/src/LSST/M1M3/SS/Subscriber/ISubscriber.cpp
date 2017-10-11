/*
 * ISubscriber.cpp
 *
 *  Created on: Sep 27, 2017
 *      Author: ccontaxis
 */

#include <ISubscriber.h>

namespace LSST {
namespace M1M3 {
namespace SS {

ISubscriber::~ISubscriber() {
	// TODO Auto-generated destructor stub
}

ICommand* ISubscriber::tryAcceptCommandStart() {
	return 0;
}

ICommand* ISubscriber::tryAcceptCommandEnable() {
	return 0;
}

ICommand* ISubscriber::tryAcceptCommandDisable() {
	return 0;
}

ICommand* ISubscriber::tryAcceptCommandStandby() {
	return 0;
}

ICommand* ISubscriber::tryAcceptCommandShutdown() {
	return 0;
}

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */
