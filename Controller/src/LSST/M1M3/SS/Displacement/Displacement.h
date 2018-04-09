/*
 * Displacement.h
 *
 *  Created on: Oct 6, 2017
 *      Author: ccontaxis
 */

#ifndef DISPLACEMENT_H_
#define DISPLACEMENT_H_

#include <DataTypes.h>

struct m1m3_IMSDataC;
struct m1m3_logevent_DisplacementSensorWarningC;

namespace LSST {
namespace M1M3 {
namespace SS {

class DisplacementSensorSettings;
struct SupportFPGAData;
class M1M3SSPublisher;
class SafetyController;

/*!
 * The class used to process displacement data.
 */
class Displacement {
private:
	DisplacementSensorSettings* displacementSensorSettings;
	SupportFPGAData* fpgaData;
	M1M3SSPublisher* publisher;
	SafetyController* safetyController;

	m1m3_IMSDataC* imsData;
	m1m3_logevent_DisplacementSensorWarningC* displacementWarning;

	uint64_t lastSampleTimestamp;
	uint64_t lastErrorTimestamp;

public:
	/*!
	 * Instantiates the displacement sensor.
	 * @param[in] displacementSensorSettings The displacement settings.
	 * @param[in] fpgaData The fpga data.
	 * @param[in] publisher The publisher.
	 * @param[in] safetyController The safety controller.
	 */
	Displacement(DisplacementSensorSettings* displacementSensorSettings, SupportFPGAData* fpgaData, M1M3SSPublisher* publisher, SafetyController* safetyController);

	/*!
	 * Processes currently available displacement sensor data and publish it.
	 */
	void processData();
};

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */

#endif /* DISPLACEMENT_H_ */
