/*
 * This file is part of LSST M1M3 support system package.
 *
 * Developed for the Vera C. Rubin Telescope and Site System.
 * This product includes software developed by the LSST Project
 * (https://www.lsst.org).
 * See the COPYRIGHT file at the top-level directory of this distribution
 * for details of code ownership.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#include <PID.h>

namespace LSST {
namespace M1M3 {
namespace SS {

PID::PID(int id, PIDParameters parameters) : _frozen(false), _offset(0) {
    _id = id;
    _initialParameters = parameters;

    _pidInfo = M1M3SSPublisher::instance().getEventPIDInfo();
    _pidData = M1M3SSPublisher::instance().getPIDData();
    updateParameters(parameters);
}

void PID::updateParameters(PIDParameters parameters) {
    _pidInfo->timestep[_id] = parameters.Timestep;
    _pidInfo->p[_id] = parameters.P;
    _pidInfo->i[_id] = parameters.I;
    _pidInfo->d[_id] = parameters.D;
    _pidInfo->n[_id] = parameters.N;
    _calculateIntermediateValues();
    _publishInfo();
}

void PID::restoreInitialParameters() {
    _pidInfo->timestep[_id] = _initialParameters.Timestep;
    _pidInfo->p[_id] = _initialParameters.P;
    _pidInfo->i[_id] = _initialParameters.I;
    _pidInfo->d[_id] = _initialParameters.D;
    _pidInfo->n[_id] = _initialParameters.N;
    _calculateIntermediateValues();
    resetPreviousValues();
    _publishInfo();
}

void PID::resetPreviousValues() {
    _pidData->errorT2[_id] = 0.0;
    _pidData->errorT1[_id] = 0.0;
    _pidData->error[_id] = 0.0;
    _pidData->controlT2[_id] = 0.0;
    _pidData->controlT1[_id] = 0.0;
    _pidData->control[_id] = 0.0;
}

double PID::process(double setpoint, double measurement) {
    _pidData->setpoint[_id] = setpoint;
    _pidData->measuredPID[_id] = measurement;
    _pidData->errorT2[_id] = _pidData->errorT1[_id];
    _pidData->errorT1[_id] = _pidData->error[_id];
    _pidData->error[_id] = _pidData->setpoint[_id] - _pidData->measuredPID[_id];
    _pidData->controlT2[_id] = _pidData->controlT1[_id];
    _pidData->controlT1[_id] = _pidData->control[_id];
    double A = _pidInfo->calculatedA[_id];
    double B = _pidInfo->calculatedB[_id];
    double C = _pidInfo->calculatedC[_id];
    double D = _pidInfo->calculatedD[_id];
    double E = _pidInfo->calculatedE[_id];
    double e = _pidData->error[_id];
    double e1 = _pidData->errorT1[_id];
    double e2 = _pidData->errorT2[_id];
    double u1 = _pidData->controlT1[_id];
    double u2 = _pidData->controlT2[_id];
    _pidData->control[_id] = D * u1 + E * u2 + A * e + B * e1 + C * e2;
    if (abs(_offset) > 0 && _frozen == false) {
        if (abs(_offset) < 21) {
            _offset = 0;
        } else {
            _offset -= _offset > 0 ? 20 : -20;
        }
    }
    return _pidData->control[_id] + _offset;
}

void PID::freeze() {
    _offset = _pidData->control[_id];
    _frozen = true;
}

void PID::publishTelemetry() {
    _pidData->timestamp = M1M3SSPublisher::instance().getTimestamp();
    M1M3SSPublisher::instance().putPIDData();
}

void PID::_calculateIntermediateValues() {
    double Kp = _pidInfo->p[_id];
    double Ki = _pidInfo->i[_id];
    double Kd = _pidInfo->d[_id];
    double N = _pidInfo->n[_id];
    double Ts = _pidInfo->timestep[_id];
    _pidInfo->calculatedA[_id] = Kp + Kd * N;
    _pidInfo->calculatedB[_id] = -2.0 * Kp + Kp * N * Ts + Ki * Ts - 2.0 * Kd * N;
    _pidInfo->calculatedC[_id] = Kp - Kp * N * Ts - Ki * Ts + Ki * N * Ts * Ts + Kd * N;
    _pidInfo->calculatedD[_id] = 2.0 - N * Ts;
    _pidInfo->calculatedE[_id] = N * Ts - 1.0;
    resetPreviousValues();
}

void PID::_publishInfo() {
    _pidInfo->timestamp = M1M3SSPublisher::instance().getTimestamp();
    M1M3SSPublisher::instance().logPIDInfo();
}

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */
