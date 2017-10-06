/*
 * CommandTypes.h
 *
 *  Created on: Sep 29, 2017
 *      Author: ccontaxis
 */

#ifndef COMMANDTYPES_H_
#define COMMANDTYPES_H_

struct Commands {
	enum Type {
		BootCommand = 0,
		StartCommand = 1,
		EnableCommand = 2,
		DisableCommand = 3,
		StandbyCommand = 4,
		UpdateCommand = 5,
		ShutdownCommand = 6
	};
};

#endif /* COMMANDTYPES_H_ */
