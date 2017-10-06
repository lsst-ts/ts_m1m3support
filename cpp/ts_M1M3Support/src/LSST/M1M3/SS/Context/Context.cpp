/*
 * Context.cpp
 *
 *  Created on: Sep 27, 2017
 *      Author: ccontaxis
 */

#include <Context.h>
#include <IStateFactory.h>
#include <IModel.h>
#include <IState.h>

namespace LSST {
namespace M1M3 {
namespace SS {

Context::Context(IStateFactory* stateFactory, IModel* model) {
	this->stateFactory = stateFactory;
	this->model = model;
	this->currentState = States::OfflineState;
}

void Context::boot(BootCommand* command) {
	IState* state = this->stateFactory->create(this->currentState);
	this->updateCurrentStateIfRequired(state->boot(command, this->model));
	this->stateFactory->destroy(state);
}

void Context::start(StartCommand* command) {
	IState* state = this->stateFactory->create(this->currentState);
	this->updateCurrentStateIfRequired(state->start(command, this->model));
	this->stateFactory->destroy(state);
}

void Context::enable(EnableCommand* command) {
	IState* state = this->stateFactory->create(this->currentState);
	this->updateCurrentStateIfRequired(state->enable(command, this->model));
	this->stateFactory->destroy(state);
}

void Context::disable(DisableCommand* command) {
	IState* state = this->stateFactory->create(this->currentState);
	this->updateCurrentStateIfRequired(state->disable(command, this->model));
	this->stateFactory->destroy(state);
}

void Context::standby(StandbyCommand* command) {
	IState* state = this->stateFactory->create(this->currentState);
	this->updateCurrentStateIfRequired(state->standby(command, this->model));
	this->stateFactory->destroy(state);
}

void Context::shutdown(ShutdownCommand* command) {
	IState* state = this->stateFactory->create(this->currentState);
	this->updateCurrentStateIfRequired(state->shutdown(command, this->model));
	this->stateFactory->destroy(state);
}

void Context::update(UpdateCommand* command) {
	IState* state = this->stateFactory->create(this->currentState);
	this->updateCurrentStateIfRequired(state->update(command, this->model));
	this->stateFactory->destroy(state);
}

void Context::updateCurrentStateIfRequired(States::Type potentialNewState) {
	if (potentialNewState != States::Ignore) {
		this->currentState = potentialNewState;
	}
}

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */
