/*
 * PID.cpp
 *
 *  Created on: Feb 13, 2018
 *      Author: ccontaxis
 */

#include <PID.h>

namespace LSST {
namespace M1M3 {
namespace SS {

PID::PID(int id, double timestep, double p, double i, double d, double n) {
	PIDParameters parameters;
	parameters.Timestep = timestep;
	parameters.P = p;
	parameters.I = i;
	parameters.D = d;
	parameters.N = n;
	this->id = id;
	this->initialParameters = parameters;
	this->parameters = this->initialParameters;
	this->a = 0;
	this->b = 0;
	this->c = 0;
	this->error = 0;
	this->errorT1 = 0;
	this->errorT2 = 0;
	this->control = 0;
	this->controlT1 = 0;
	this->setpoint = 0;
	this->measurement = 0;
	this->calculateIntermediateValues();
	this->publishInfo();
}

PID::PID(int id, PIDParameters parameters) {
	this->id = id;
	this->initialParameters = parameters;
	this->parameters = this->initialParameters;
	this->a = 0;
	this->b = 0;
	this->c = 0;
	this->error = 0;
	this->errorT1 = 0;
	this->errorT2 = 0;
	this->control = 0;
	this->controlT1 = 0;
	this->setpoint = 0;
	this->measurement = 0;
	this->calculateIntermediateValues();
	this->publishInfo();
}

void PID::updateParameters(PIDParameters parameters) {
	this->parameters = parameters;
	this->calculateIntermediateValues();
	this->publishInfo();
}

void PID::restoreInitialParameters() {
	this->parameters = this->initialParameters;
	this->calculateIntermediateValues();
	this->resetPreviousValues();
	this->publishInfo();
}

void PID::resetPreviousValues() {
	this->error = 0;
	this->errorT1 = 0;
	this->errorT2 = 0;
	this->control = 0;
	this->controlT1 = 0;
}

double PID::process(double setpoint, double measurement) {
	this->setpoint = setpoint;
	this->measurement = measurement;
	this->error = this->setpoint - this->measurement;
	this->control = this->controlT1 + this->a * this->error + this->b * this->errorT1 + this->c * this->errorT2;
	this->controlT1 = this->control;
	this->errorT2 = this->errorT1;
	this->errorT1 = this->error;
	return this->control;
}

void PID::publishTelemetry() {

}

void PID::calculateIntermediateValues() {
	this->a = this->parameters.P;
	this->b = (-this->parameters.P) + this->parameters.I * this->parameters.Timestep;
	this->c = this->parameters.D;
	this->resetPreviousValues();
}

void PID::publishInfo() {

}

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */
