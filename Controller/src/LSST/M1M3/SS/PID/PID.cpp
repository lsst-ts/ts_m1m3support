/*
 * PID.cpp
 *
 *  Created on: Feb 13, 2018
 *      Author: ccontaxis
 */

#include <PID.h>
#include <M1M3SSPublisher.h>
#include <SAL_m1m3C.h>

namespace LSST {
namespace M1M3 {
namespace SS {

PID::PID(int id, PIDParameters parameters, M1M3SSPublisher* publisher) {
	this->id = id;
	this->initialParameters = parameters;
	this->publisher = publisher;
	this->pidInfo = this->publisher->getEventPIDInfo();
	this->pidData = this->publisher->getPIDData();
	this->parameters = this->initialParameters;
	this->a = 0;
	this->b = 0;
	this->c = 0;
	this->d = 0;
	this->e = 0;
	this->error = 0;
	this->errorT1 = 0;
	this->errorT2 = 0;
	this->control = 0;
	this->controlT1 = 0;
	this->controlT2 = 0;
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
	this->controlT2 = 0;
}

double PID::process(double setpoint, double measurement) {
	this->setpoint = setpoint;
	this->measurement = measurement;
	this->error = this->setpoint - this->measurement;
	this->control = this->d * this->controlT1 + this->e * this->controlT2 + this->a * this->error + this->b * this->errorT1 + this->c * this->errorT2;
	this->controlT2 = this->controlT1;
	this->controlT1 = this->control;
	this->errorT2 = this->errorT1;
	this->errorT1 = this->error;
	this->pidData->Setpoint[this->id] = setpoint;
	this->pidData->Measurement[this->id] = measurement;
	this->pidData->Error[this->id] = this->error;
	this->pidData->ErrorT1[this->id] = this->errorT1;
	this->pidData->ErrorT2[this->id] = this->errorT2;
	this->pidData->Control[this->id] = this->control;
	this->pidData->ControlT1[this->id] = this->controlT1;
	this->pidData->ControlT2[this->id] = this->controlT2;
	return this->control;
}

void PID::publishTelemetry() {
	this->pidData->Timestamp = this->publisher->getTimestamp();
	this->publisher->putPIDData();
}

void PID::calculateIntermediateValues() {
	this->a = this->parameters.P + this->parameters.D * this->parameters.N;
	this->b = -2  * this->parameters.P + this->parameters.P * this->parameters.N * this->parameters.Timestep + this->parameters.I * this->parameters.Timestep - 2 * this->parameters.D * this->parameters.N;
	this->c = this->parameters.P - this->parameters.P * this->parameters.N * this->parameters.Timestep - this->parameters.I * this->parameters.Timestep + this->parameters.I * this->parameters.N * this->parameters.Timestep * this->parameters.Timestep + this->parameters.D * this->parameters.N;
	this->d = 2 - this->parameters.N * this->parameters.Timestep;
	this->e = this->parameters.N * this->parameters.Timestep - 1;
	this->resetPreviousValues();
}

void PID::publishInfo() {
	this->pidInfo->Timestamp = this->publisher->getTimestamp();
	this->pidInfo->Timestep[this->id] = this->parameters.Timestep;
	this->pidInfo->P[this->id] = this->parameters.P;
	this->pidInfo->I[this->id] = this->parameters.I;
	this->pidInfo->D[this->id] = this->parameters.D;
	this->pidInfo->N[this->id] = this->parameters.N;
	this->pidInfo->CalculatedA[this->id] = this->a;
	this->pidInfo->CalculatedB[this->id] = this->b;
	this->pidInfo->CalculatedC[this->id] = this->c;
	this->pidInfo->CalculatedD[this->id] = this->d;
	this->pidInfo->CalculatedE[this->id] = this->e;
	this->publisher->logPIDInfo();
}

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */
