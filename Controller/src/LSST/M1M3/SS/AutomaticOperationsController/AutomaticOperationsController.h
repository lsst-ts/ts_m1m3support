/*
 * AutomaticOperationsController.h
 *
 *  Created on: Dec 13, 2017
 *      Author: ccontaxis
 */

#ifndef AUTOMATICOPERATIONSCONTROLLER_H_
#define AUTOMATICOPERATIONSCONTROLLER_H_

#include <IAutomaticOperationsController.h>

namespace LSST {
namespace M1M3 {
namespace SS {

class IPositionController;
class IForceController;
class IInterlockController;
class ISafetyController;
class IPublisher;
class IPowerController;

class AutomaticOperationsController: public IAutomaticOperationsController {
private:
	IPositionController* positionController;
	IForceController* forceController;
	IInterlockController* interlockController;
	ISafetyController* safetyController;
	IPublisher* publisher;
	IPowerController* powerController;
	double cachedTimestamp;

public:
	AutomaticOperationsController(IPositionController* positionController, IForceController* forceController, IInterlockController* interlockController, ISafetyController* safetyController, IPublisher* publisher, IPowerController* powerController);

	void startRaiseOperation();
	void tryIncrementingSupportPercentage();
	bool checkRaiseOperationComplete();
	void completeRaiseOperation();
	bool checkRaiseOperationTimeout();
	void timeoutRaiseOperation();

	void startLowerOperation();
	void tryDecrementSupportPercentage();
	bool checkLowerOperationComplete();
	void completeLowerOperation();
	bool checkLowerOperationTimeout();
	void timeoutLowerOperation();

	void uncontrolledLowerOperation();
};

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */

#endif /* AUTOMATICOPERATIONSCONTROLLER_H_ */
