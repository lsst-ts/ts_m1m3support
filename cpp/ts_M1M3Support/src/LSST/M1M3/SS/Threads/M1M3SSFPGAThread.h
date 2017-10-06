/*
 * M1M3SSFPGAThread.h
 *
 *  Created on: Sep 28, 2017
 *      Author: ccontaxis
 */

#ifndef M1M3SSFPGATHREAD_H_
#define M1M3SSFPGATHREAD_H_

#include <IThread.h>

namespace LSST {
namespace M1M3 {
namespace SS {

class M1M3SSFPGAThread: public IThread {
public:
	virtual ~M1M3SSFPGAThread();
};

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */

#endif /* M1M3SSFPGATHREAD_H_ */
