/*
 * HardpointActuatorSettings.h
 *
 *  Created on: Oct 25, 2017
 *      Author: ccontaxis
 */

#ifndef HARDPOINTACTUATORSETTINGS_H_
#define HARDPOINTACTUATORSETTINGS_H_

#include <DataTypes.h>
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
	int32_t HP1EncoderOffset;
	int32_t HP2EncoderOffset;
	int32_t HP3EncoderOffset;
	int32_t HP4EncoderOffset;
	int32_t HP5EncoderOffset;
	int32_t HP6EncoderOffset;
	double XPositionOffset;
	double YPositionOffset;
	double ZPositionOffset;
	double XRotationOffset;
	double YRotationOffset;
	double ZRotationOffset;

	void load(const std::string &filename);
};

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */

#endif /* HARDPOINTACTUATORSETTINGS_H_ */
