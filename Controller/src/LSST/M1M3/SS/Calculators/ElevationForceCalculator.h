/*
 * ElevationForceCalculator.h
 *
 *  Created on: Oct 23, 2017
 *      Author: ccontaxis
 */

#ifndef ELEVATIONFORCECALCULATOR_H_
#define ELEVATIONFORCECALCULATOR_H_

struct m1m3_ElevationDataC;
struct m1m3_ForceActuatorDataC;

namespace LSST {
namespace M1M3 {
namespace SS {

class ForceActuatorApplicationSettings;

class ElevationForceCalculator {
private:
	ForceActuatorApplicationSettings* forceActuatorSettings;
	m1m3_InclinometerDataC* inclinometerData;
	m1m3_ForceActuatorDataC* forceData;

public:
	ElevationForceCalculator(ForceActuatorApplicationSettings* forceActuatorSettings, m1m3_InclinometerDataC* inclinometerData, m1m3_ForceActuatorDataC* forceData);

	void calculate();
};

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */

#endif /* ELEVATIONFORCECALCULATOR_H_ */
