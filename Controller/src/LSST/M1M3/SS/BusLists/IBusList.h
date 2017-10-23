/*
 * IBusList.h
 *
 *  Created on: Oct 19, 2017
 *      Author: ccontaxis
 */

#ifndef IBUSLIST_H_
#define IBUSLIST_H_

#include <DataTypes.h>

namespace LSST {
namespace M1M3 {
namespace SS {

class IBusList {
public:
	virtual ~IBusList();

	virtual int32_t getLength();
	virtual uint16_t* getBuffer();

	virtual int32_t* getExpectedHPResponses();
	virtual int32_t* getExpectedFAResponses();

	virtual void update();
};

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */

#endif /* IBUSLIST_H_ */
