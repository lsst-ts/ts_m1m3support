/*
 * Command.h
 *
 *  Created on: Sep 26, 2017
 *      Author: ccontaxis
 */

#ifndef COMMAND_H_
#define COMMAND_H_

#include <string>
#include <DataTypes.h>

namespace LSST {
namespace M1M3 {
namespace SS {

class Context;
class M1M3SSPublisher;

class Command {
protected:
    int32_t commandID;

public:
    virtual ~Command();

    /*!
     * Gets the command ID.
     */
    virtual int32_t getCommandID() { return this->commandID; }

    /*!
     * Validates the command.
     */
    virtual bool validate();
    /*!
     * Executes the command.
     */
    virtual void execute();
    /*!
     * Acknowledges the command is in progress.
     */
    virtual void ackInProgress();
    /*!
     * Acknowledges the command has completed successfully.
     */
    virtual void ackComplete();
    /*!
     * Acknowledges the command has failed.
     * @param[in] reason The reason why the command has failed.
     */
    virtual void ackFailed(std::string reason);
};

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */

#endif /* COMMAND_H_ */
