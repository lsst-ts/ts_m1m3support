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
    Context* context;

public:
    virtual ~Command();

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
	 * Acknowledges the command as not permitted.
	 * @parma[in] errorCode The error code.
	 * @param[in] reason The reason why the command has failed.
	 */
    virtual void ackNotPermitted(int32_t errorCode, std::string reason);
    /*!
	 * Acknowledges the command as aborted.
	 * @parma[in] errorCode The error code.
	 * @param[in] reason The reason why the command has failed.
	 */
    virtual void ackAborted(int32_t errorCode, std::string reason);
    /*!
	 * Acknowledges the command has failed.
	 * @parma[in] errorCode The error code.
	 * @param[in] reason The reason why the command has failed.
	 */
    virtual void ackFailed(int32_t errorCode, std::string reason);
    /*!
	 * Acknowledges the command as being executed in an invalid state.
	 * @param[in] reason The reason why the command has failed.
	 */
    virtual void ackInvalidState(std::string reason);
    /*!
	 * Acknowledges the command as having an invalid parameter.
	 * @param[in] reason The reason why the command has failed.
	 */
    virtual void ackInvalidParameter(std::string reason);
    /*!
	 * Acknowledges the command is already in progress.
	 * @param[in] reason The reason why the command has failed.
	 */
    virtual void ackAlreadyInProgress(std::string reason);
    /*!
	 * Acknowledges the command as being blocked by other command.
	 * @param[in] reason The reason why the command has failed.
	 */
    virtual void ackExecutionBlocked(std::string reason);
    /*!
	 * Acknowledges the command as already being in this state.
	 * @param[in] reason The reason why the command has failed.
	 */
    virtual void ackAlreadyInState(std::string reason);
    /*!
	 * Acknowledges the command.
	 * @param[in] ackCode The ack code.
	 * @param[in] errorCode The error code.
	 * @param[in] description The description.
	 */
    virtual void ack(int32_t ackCode, int32_t errorCode, std::string description);
};

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */

#endif /* COMMAND_H_ */
