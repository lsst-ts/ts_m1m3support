/*
 * IAutomaticOperationsController.h
 *
 *  Created on: Dec 13, 2017
 *      Author: ccontaxis
 */

#ifndef IAUTOMATICOPERATIONSCONTROLLER_H_
#define IAUTOMATICOPERATIONSCONTROLLER_H_

namespace LSST {
namespace M1M3 {
namespace SS {

class IAutomaticOperationsController {
public:
	virtual ~IAutomaticOperationsController();

	virtual void startRaiseOperation();
	virtual void tryIncrementingSupportPercentage();
	virtual bool checkRaiseOperationComplete();
	virtual void completeRaiseOperation();
	virtual bool checkRaiseOperationTimeout();
	virtual void timeoutRaiseOperation();

	virtual void startLowerOperation();
	virtual void tryDecrementSupportPercentage();
	virtual bool checkLowerOperationComplete();
	virtual void completeLowerOperation();
	virtual bool checkLowerOperationTimeout();
	virtual void timeoutLowerOperation();
};

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */

#endif /* IAUTOMATICOPERATIONSCONTROLLER_H_ */
