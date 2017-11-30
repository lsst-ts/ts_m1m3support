/*
 * State.h
 *
 *  Created on: Sep 26, 2017
 *      Author: ccontaxis
 */

#ifndef STATE_H_
#define STATE_H_

#include <IState.h>
#include <time.h>

namespace LSST {
namespace M1M3 {
namespace SS {

class State: public IState {
private:
	clockid_t clk_id;
	timespec start;
	timespec stop;

protected:
	void startTimer();
	void stopTimer();
	double getTimer();
};

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */

#endif /* STATE_H_ */
