/*
 * AutomaticOperationsController.h
 *
 *  Created on: Dec 13, 2017
 *      Author: ccontaxis
 */

#ifndef AUTOMATICOPERATIONSCONTROLLER_H_
#define AUTOMATICOPERATIONSCONTROLLER_H_

namespace LSST {
namespace M1M3 {
namespace SS {

class ForceController;
class InterlockController;
class M1M3SSPublisher;
class PositionController;
class PowerController;
class SafetyController;

class AutomaticOperationsController {
private:
	PositionController* positionController;
	ForceController* forceController;
	InterlockController* interlockController;
	SafetyController* safetyController;
	M1M3SSPublisher* publisher;
	PowerController* powerController;
	double cachedTimestamp;
	bool bypassMoveToReference;

public:
	AutomaticOperationsController(PositionController* positionController, ForceController* forceController, InterlockController* interlockController, SafetyController* safetyController, M1M3SSPublisher* publisher, PowerController* powerController);

	void startRaiseOperation(bool bypassMoveToReference);
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
