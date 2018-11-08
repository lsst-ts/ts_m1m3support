/*
 * PPSThread.h
 *
 *  Created on: Mar 29, 2018
 *      Author: ccontaxis
 */

#ifndef PPSTHREAD_H_
#define PPSTHREAD_H_

#include <IThread.h>
#include <DataTypes.h>

namespace LSST {
namespace M1M3 {
namespace SS {

class IFPGA;
class M1M3SSPublisher;

class PPSThread: public IThread {
private:
	IFPGA* fpga;
	M1M3SSPublisher* publisher;
	uint16_t timestampUpdateBuffer[5];
	bool keepRunning;

public:
	PPSThread(IFPGA* fpga, M1M3SSPublisher* publisher);

	void run();
	void stop();
};

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */

#endif /* PPSTHREAD_H_ */
