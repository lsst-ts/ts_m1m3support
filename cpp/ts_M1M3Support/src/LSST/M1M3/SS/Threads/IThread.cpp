/*
 * IThread.cpp
 *
 *  Created on: Sep 28, 2017
 *      Author: ccontaxis
 */

#include <IThread.h>
#include <iostream>

namespace LSST {
namespace M1M3 {
namespace SS {

IThread::~IThread() {
	// TODO Auto-generated destructor stub
}

void IThread::run() {
	std::cout << "IThread::run" << std::endl;
}

void IThread::stop() {

}

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */
