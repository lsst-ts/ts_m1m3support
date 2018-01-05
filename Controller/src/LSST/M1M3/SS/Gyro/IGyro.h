/*
 * IGyro.h
 *
 *  Created on: Jan 4, 2018
 *      Author: ccontaxis
 */

#ifndef IGYRO_H_
#define IGYRO_H_

namespace LSST {
namespace M1M3 {
namespace SS {

class IGyro {
public:
	virtual ~IGyro();

	virtual void enterConfigurationMode();
	virtual void exitConfigurationMode();

	virtual void resetConfiguration();

	virtual void setRotationFormatRate();
	virtual void setRotationUnitsRadians();
	virtual void setAxis();
	virtual void setDataRate();
};

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */

#endif /* IGYRO_H_ */
