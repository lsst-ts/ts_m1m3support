/*
 * IAirController.h
 *
 *  Created on: Oct 16, 2017
 *      Author: ccontaxis
 */

#ifndef IAIRCONTROLLER_H_
#define IAIRCONTROLLER_H_

namespace LSST {
namespace M1M3 {
namespace SS {

/*!
 * Class used for controlling air supply.
 */
class IAirController {
public:
	virtual ~IAirController();

	/*!
	 * Turns the air on in the cell.
	 */
	virtual void turnAirOn();
	/*!
	 * Turns the air off in the cell.
	 */
	virtual void turnAirOff();
	/*!
	 * Checks the air status.
	 */
	virtual void checkStatus();
};

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */

#endif /* IAIRCONTROLLER_H_ */
