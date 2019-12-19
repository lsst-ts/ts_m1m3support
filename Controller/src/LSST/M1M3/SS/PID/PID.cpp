/*
 * PID.cpp
 *
 *  Created on: Feb 13, 2018
 *      Author: ccontaxis
 */

#include <PID.h>
#include <M1M3SSPublisher.h>
#include <SAL_MTM1M3C.h>

namespace LSST {
namespace M1M3 {
namespace SS {

PID::PID(int id, PIDParameters parameters, M1M3SSPublisher* publisher) {
    this->id = id;
    this->initialParameters = parameters;
    this->publisher = publisher;
    this->pidInfo = this->publisher->getEventPIDInfo();
    this->pidData = this->publisher->getPIDData();
    this->pidInfo->timestep[this->id] = parameters.Timestep;
    this->pidInfo->p[this->id] = parameters.P;
    this->pidInfo->i[this->id] = parameters.I;
    this->pidInfo->d[this->id] = parameters.D;
    this->pidInfo->n[this->id] = parameters.N;
    this->calculateIntermediateValues();
    this->publishInfo();
}

void PID::updateParameters(PIDParameters parameters) {
    this->pidInfo->timestep[this->id] = parameters.Timestep;
    this->pidInfo->p[this->id] = parameters.P;
    this->pidInfo->i[this->id] = parameters.I;
    this->pidInfo->d[this->id] = parameters.D;
    this->pidInfo->n[this->id] = parameters.N;
    this->calculateIntermediateValues();
    this->publishInfo();
}

void PID::restoreInitialParameters() {
    this->pidInfo->timestep[this->id] = this->initialParameters.Timestep;
    this->pidInfo->p[this->id] = this->initialParameters.P;
    this->pidInfo->i[this->id] = this->initialParameters.I;
    this->pidInfo->d[this->id] = this->initialParameters.D;
    this->pidInfo->n[this->id] = this->initialParameters.N;
    this->calculateIntermediateValues();
    this->resetPreviousValues();
    this->publishInfo();
}

void PID::resetPreviousValues() {
    this->pidData->errorT2[this->id] = 0.0;
    this->pidData->errorT1[this->id] = 0.0;
    this->pidData->error[this->id] = 0.0;
    this->pidData->controlT2[this->id] = 0.0;
    this->pidData->controlT1[this->id] = 0.0;
    this->pidData->control[this->id] = 0.0;
}

double PID::process(double setpoint, double measurement) {
    this->pidData->setpoint[this->id] = setpoint;
    this->pidData->measurementValue[this->id] = measurement;
    this->pidData->errorT2[this->id] = this->pidData->errorT1[this->id];
    this->pidData->errorT1[this->id] = this->pidData->error[this->id];
    this->pidData->error[this->id] = this->pidData->setpoint[this->id] - this->pidData->measurementValue[this->id];
    this->pidData->controlT2[this->id] = this->pidData->controlT1[this->id];
    this->pidData->controlT1[this->id] = this->pidData->control[this->id];
    double A = this->pidInfo->calculatedA[this->id];
    double B = this->pidInfo->calculatedB[this->id];
    double C = this->pidInfo->calculatedC[this->id];
    double D = this->pidInfo->calculatedD[this->id];
    double E = this->pidInfo->calculatedE[this->id];
    double e = this->pidData->error[this->id];
    double e1 = this->pidData->errorT1[this->id];
    double e2 = this->pidData->errorT2[this->id];
    double u1 = this->pidData->controlT1[this->id];
    double u2 = this->pidData->controlT2[this->id];
    this->pidData->control[this->id] = D * u1 + E * u2 + A * e + B * e1 + C * e2;
    return this->pidData->control[this->id];
}

void PID::publishTelemetry() {
    this->pidData->timestamp = this->publisher->getTimestamp();
    this->publisher->putPIDData();
}

void PID::calculateIntermediateValues() {
    double Kp = this->pidInfo->p[this->id];
    double Ki = this->pidInfo->i[this->id];
    double Kd = this->pidInfo->d[this->id];
    double N = this->pidInfo->n[this->id];
    double Ts = this->pidInfo->timestep[this->id];
    this->pidInfo->calculatedA[this->id] = Kp + Kd * N;
    this->pidInfo->calculatedB[this->id] = -2.0 * Kp + Kp * N * Ts + Ki * Ts - 2.0 * Kd * N;
    this->pidInfo->calculatedC[this->id] = Kp - Kp * N * Ts - Ki * Ts + Ki * N * Ts * Ts + Kd * N;
    this->pidInfo->calculatedD[this->id] = 2.0 - N * Ts;
    this->pidInfo->calculatedE[this->id] = N * Ts - 1.0;
    this->resetPreviousValues();
}

void PID::publishInfo() {
    this->publisher->logPIDInfo();
}

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */
