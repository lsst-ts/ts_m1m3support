/*
 * IILC.h
 *
 *  Created on: Oct 3, 2017
 *      Author: ccontaxis
 */

#ifndef IILC_H_
#define IILC_H_

#include <DataTypes.h>

namespace LSST {
namespace M1M3 {
namespace SS {

class IILC {
public:
	virtual ~IILC();

	virtual void writeCalibrationDataBuffer();
	virtual void writeSetADCScanRateBuffer();
	virtual void writeSetBoostValveDCAGainBuffer();
	virtual void writeResetBuffer();
	virtual void writeReportServerIDBuffer();
	virtual void writeReportServerStatusBuffer();
	virtual void writeReportADCScanRateBuffer();
	virtual void writeReadCalibrationDataBuffer();
	virtual void writeReadBoostValveDCAGainBuffer();
	virtual void writeReportDCAIDBuffer();
	virtual void writeReportDCAStatusBuffer();
	virtual void writeSetModeStandbyBuffer();
	virtual void writeSetModeDisableBuffer();
	virtual void writeSetModeEnableBuffer();
	virtual void writeFreezeSensorListBuffer();

	virtual void waitForSubnet(int32_t subnet, int32_t timeout);
	virtual void waitForAllSubnets(int32_t timeout);

	virtual void read(uint8_t subnet);
	virtual void readAll();
};

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */

#endif /* IILC_H_ */
