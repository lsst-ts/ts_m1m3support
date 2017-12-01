/*
 * State.cpp
 *
 *  Created on: Sep 26, 2017
 *      Author: ccontaxis
 */

#include <State.h>
#include <DataTypes.h>

namespace LSST {
namespace M1M3 {
namespace SS {

void State::startTimer() {
	clock_gettime(CLOCK_REALTIME, &this->start);
}

void State::stopTimer() {
	clock_gettime(CLOCK_REALTIME, &this->stop);
}

double State::getTimer() {
	double deltaNano = this->stop.tv_nsec - this->start.tv_nsec;
	double deltaSec = this->stop.tv_sec - this->start.tv_sec;
	if (deltaNano < 0) {
		deltaSec -= 1;
		deltaNano += 1000000000;
	}
	return deltaSec + (deltaNano / 1000000000.0);
}

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */
