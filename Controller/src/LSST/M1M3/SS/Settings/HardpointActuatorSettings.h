/*
 * HardpointActuatorSettings.h
 *
 *  Created on: Oct 25, 2017
 *      Author: ccontaxis
 */

#ifndef HARDPOINTACTUATORSETTINGS_H_
#define HARDPOINTACTUATORSETTINGS_H_

#include <string>
#include <vector>

namespace LSST {
namespace M1M3 {
namespace SS {

class HardpointActuatorSettings {
public:
	std::vector<double> HardpointDisplacementToMirrorPosition;
	std::vector<double> MirrorPositionToHardpointDisplacement;
	double MicrometersPerStep;
	double MicrometersPerEncoder;
	double DisplacementOffset1;
	double DisplacementOffset2;
	double DisplacementOffset3;
	double DisplacementOffset4;
	double DisplacementOffset5;
	double DisplacementOffset6;

	void load(const std::string &filename);
};

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */

#endif /* HARDPOINTACTUATORSETTINGS_H_ */
