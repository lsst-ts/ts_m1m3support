/*
 * ElevationForceCalculator.cpp
 *
 *  Created on: Oct 23, 2017
 *      Author: ccontaxis
 */

#include <ElevationForceCalculator.h>
#include <SAL_m1m3C.h>
#include <cmath>
#include <vector>

namespace LSST {
namespace M1M3 {
namespace SS {

ElevationForceCalculator::ElevationForceCalculator(ForceActuatorApplicationSettings* forceActuatorSettings, m1m3_InclinometerDataC* inclinometerData, m1m3_ForceActuatorDataC* forceData) {
	this->forceActuatorSettings = forceActuatorSettings;
	this->inclinometerData = inclinometerData;
	this->forceData = forceData;
}

void ElevationForceCalculator::calculate() {
	double elevationAngle = this->inclinometerData->InclinometerAngle;
	double elevationMatrix[] = { std::pow(elevationAngle, 5.0), std::pow(elevationAngle, 4.0), std::pow(elevationAngle, 3.0), std::pow(elevationAngle, 2.0), elevationAngle, 1 };
	for(int i = 0; i < 156; ++i) {
		int mIndex = i * 6;
		this->forceData->ElevationXSetpoint[i] =
				this->forceActuatorSettings->ElevationXAxisCoefficients[mIndex + 0] * elevationMatrix[0] +
				this->forceActuatorSettings->ElevationXAxisCoefficients[mIndex + 1] * elevationMatrix[1] +
				this->forceActuatorSettings->ElevationXAxisCoefficients[mIndex + 2] * elevationMatrix[2] +
				this->forceActuatorSettings->ElevationXAxisCoefficients[mIndex + 3] * elevationMatrix[3] +
				this->forceActuatorSettings->ElevationXAxisCoefficients[mIndex + 4] * elevationMatrix[4] +
				this->forceActuatorSettings->ElevationXAxisCoefficients[mIndex + 5];
		this->forceData->ElevationYSetpoint[i] =
				this->forceActuatorSettings->ElevationYAxisCoefficients[mIndex + 0] * elevationMatrix[0] +
				this->forceActuatorSettings->ElevationYAxisCoefficients[mIndex + 1] * elevationMatrix[1] +
				this->forceActuatorSettings->ElevationYAxisCoefficients[mIndex + 2] * elevationMatrix[2] +
				this->forceActuatorSettings->ElevationYAxisCoefficients[mIndex + 3] * elevationMatrix[3] +
				this->forceActuatorSettings->ElevationYAxisCoefficients[mIndex + 4] * elevationMatrix[4] +
				this->forceActuatorSettings->ElevationYAxisCoefficients[mIndex + 5];
		this->forceData->ElevationZSetpoint[i] =
				this->forceActuatorSettings->ElevationZAxisCoefficients[mIndex + 0] * elevationMatrix[0] +
				this->forceActuatorSettings->ElevationZAxisCoefficients[mIndex + 1] * elevationMatrix[1] +
				this->forceActuatorSettings->ElevationZAxisCoefficients[mIndex + 2] * elevationMatrix[2] +
				this->forceActuatorSettings->ElevationZAxisCoefficients[mIndex + 3] * elevationMatrix[3] +
				this->forceActuatorSettings->ElevationZAxisCoefficients[mIndex + 4] * elevationMatrix[4] +
				this->forceActuatorSettings->ElevationZAxisCoefficients[mIndex + 5];
	}
}

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */
