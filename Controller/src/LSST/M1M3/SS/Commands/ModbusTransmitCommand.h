/*
 * ModbusTransmitCommand.h
 *
 *  Created on: May 18, 2018
 *      Author: ccontaxis
 */

#ifndef MODBUSTRANSMITCOMMAND_H_
#define MODBUSTRANSMITCOMMAND_H_

#include <SALCommand.h>
#include <SAL_MTM1M3C.h>
#include <DataTypes.h>

namespace LSST {
namespace M1M3 {
namespace SS {

class ModbusTransmitCommand: public SALCommand<MTM1M3_command_modbusTransmitC> {
public:
	ModbusTransmitCommand(Context* context, M1M3SSPublisher* publisher, int32_t commandID, MTM1M3_command_modbusTransmitC* data);

	void execute();
	void ack(int32_t ack, int32_t errorCode, std::string reason);
};

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */

#endif /* MODBUSTRANSMITCOMMAND_H_ */
