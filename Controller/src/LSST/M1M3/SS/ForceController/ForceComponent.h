/*
 * ForceComponent.h
 *
 *  Created on: Jul 6, 2018
 *      Author: ccontaxis
 */

#ifndef LSST_M1M3_SS_FORCECONTROLLER_FORCECOMPONENT_H_
#define LSST_M1M3_SS_FORCECONTROLLER_FORCECOMPONENT_H_

namespace LSST {
namespace M1M3 {
namespace SS {

class ForceComponent {
protected:
	bool enabled;
	bool disabling;
	float maxLimit;

	float xCurrent[12];
	float yCurrent[100];
	float zCurrent[156];

	float xTarget[12];
	float yTarget[100];
	float zTarget[156];

	float xOffset[12];
	float yOffset[100];
	float zOffset[156];

public:
	ForceComponent();
	virtual ~ForceComponent();

	bool isEnabled();

	void enable();
	void disable();

	void update();

protected:
	virtual void postEnableDisableActions();
	virtual void postUpdateActions();
};

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */

#endif /* LSST_M1M3_SS_FORCECONTROLLER_FORCECOMPONENT_H_ */
