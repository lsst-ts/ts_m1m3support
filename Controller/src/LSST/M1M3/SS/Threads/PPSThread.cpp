/*
 * PPSThread.cpp
 *
 *  Created on: Mar 29, 2018
 *      Author: ccontaxis
 */

#include <PPSThread.h>
#include <FPGA.h>
#include <M1M3SSPublisher.h>
#include <Log.h>
#include <Timestamp.h>
#include <FPGAAddresses.h>

namespace LSST {
namespace M1M3 {
namespace SS {

PPSThread::PPSThread(FPGA* fpga, M1M3SSPublisher* publisher) {
	this->fpga = fpga;
	this->publisher = publisher;
	this->keepRunning = true;
}

void PPSThread::run() {
	Log.Info("PPSThread: Start");
	while(this->keepRunning) {
		if (this->fpga->waitForPPS(2500) == 0) {
			this->fpga->ackPPS();
			uint64_t timestamp = Timestamp::toFPGA(this->publisher->getTimestamp());
			if (this->keepRunning) {
				this->fpga->writeTimestampFIFO(timestamp);
			}
		}
		else {
			Log.Warn("PPSThread: Failed to receive pps");
		}
	}
	Log.Info("PPSThread: Completed");
}

void PPSThread::stop() {
	this->keepRunning = false;
}

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */
