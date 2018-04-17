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
	this->pidInfo->Timestep[this->id] = parameters.Timestep;
	this->pidInfo->P[this->id] = parameters.P;
	this->pidInfo->I[this->id] = parameters.I;
	this->pidInfo->D[this->id] = parameters.D;
	this->pidInfo->N[this->id] = parameters.N;
	this->calculateIntermediateValues();
	this->publishInfo();
}

void PID::updateParameters(PIDParameters parameters) {
	this->pidInfo->Timestep[this->id] = parameters.Timestep;
	this->pidInfo->P[this->id] = parameters.P;
	this->pidInfo->I[this->id] = parameters.I;
	this->pidInfo->D[this->id] = parameters.D;
	this->pidInfo->N[this->id] = parameters.N;
	this->calculateIntermediateValues();
	this->publishInfo();
}

void PID::restoreInitialParameters() {
	this->pidInfo->Timestep[this->id] = this->initialParameters.Timestep;
	this->pidInfo->P[this->id] = this->initialParameters.P;
	this->pidInfo->I[this->id] = this->initialParameters.I;
	this->pidInfo->D[this->id] = this->initialParameters.D;
	this->pidInfo->N[this->id] = this->initialParameters.N;
	this->calculateIntermediateValues();
	this->resetPreviousValues();
	this->publishInfo();
}

void PID::resetPreviousValues() {
	this->pidData->ErrorT2[this->id] = 0.0;
	this->pidData->ErrorT1[this->id] = 0.0;
	this->pidData->Error[this->id] = 0.0;
	this->pidData->ControlT2[this->id] = 0.0;
	this->pidData->ControlT1[this->id] = 0.0;
	this->pidData->Control[this->id] = 0.0;
}

double PID::process(double setpoint, double measurement) {
	this->pidData->Setpoint[this->id] = setpoint;
	this->pidData->Measurement[this->id] = measurement;
	this->pidData->ErrorT2[this->id] = this->pidData->ErrorT1[this->id];
	this->pidData->ErrorT1[this->id] = this->pidData->Error[this->id];
	this->pidData->Error[this->id] = this->pidData->Setpoint[this->id] - this->pidData->Measurement[this->id];
	this->pidData->ControlT2[this->id] = this->pidData->ControlT1[this->id];
	this->pidData->ControlT1[this->id] = this->pidData->Control[this->id];
	double A = this->pidInfo->CalculatedA[this->id];
	double B = this->pidInfo->CalculatedB[this->id];
	double C = this->pidInfo->CalculatedC[this->id];
	double D = this->pidInfo->CalculatedD[this->id];
	double E = this->pidInfo->CalculatedE[this->id];
	double e = this->pidData->Error[this->id];
	double e1 = this->pidData->ErrorT1[this->id];
	double e2 = this->pidData->ErrorT2[this->id];
	double u1 = this->pidData->ControlT1[this->id];
	double u2 = this->pidData->ControlT2[this->id];
	this->pidData->Control[this->id] = D * u1 + E * u2 + A * e + B * e1 + C * e2;
	return this->pidData->Control[this->id];
}

void PID::publishTelemetry() {
	this->pidData->Timestamp = this->publisher->getTimestamp();
	this->publisher->putPIDData();
}

void PID::calculateIntermediateValues() {
	double Kp = this->pidInfo->P[this->id];
	double Ki = this->pidInfo->I[this->id];
	double Kd = this->pidInfo->D[this->id];
	double N = this->pidInfo->N[this->id];
	double Ts = this->pidInfo->Timestep[this->id];
	this->pidInfo->CalculatedA[this->id] = Kp + Kd * N;
	this->pidInfo->CalculatedB[this->id] = -2.0 * Kp + Kp * N * Ts + Ki * Ts - 2.0 * Kd * N;
	this->pidInfo->CalculatedC[this->id] = Kp - Kp * N * Ts - Ki * Ts + Ki * N * Ts * Ts + Kd * N;
	this->pidInfo->CalculatedD[this->id] = 2.0 - N * Ts;
	this->pidInfo->CalculatedE[this->id] = N * Ts - 1.0;
	this->resetPreviousValues();
}

void PID::publishInfo() {
	this->pidInfo->Timestamp = this->publisher->getTimestamp();
	this->publisher->logPIDInfo();
}

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */
