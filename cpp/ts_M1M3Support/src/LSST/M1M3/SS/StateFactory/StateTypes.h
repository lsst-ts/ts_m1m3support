/*
 * StateTypes.h
 *
 *  Created on: Sep 26, 2017
 *      Author: ccontaxis
 */

#ifndef STATETYPES_H_
#define STATETYPES_H_

struct States {
	enum Type {
		OfflineState = 0,
		StandbyState = 1,
		DisabledState = 2,
		EnabledState = 3,
		Ignore = 999
	};
};

#endif /* STATETYPES_H_ */
