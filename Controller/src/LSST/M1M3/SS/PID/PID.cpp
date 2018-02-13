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

PID::PID(double p, double i, double d, double timestep) {
	this->p = p;
	this->i = i;
	this->d = d;
	this->timestep = timestep;
	this->a = this->p;
	this->b = (-this->p) + this->i * this->timestep;
	this->c = this->d;
	this->error = 0;
	this->errorT1 = 0;
	this->errorT2 = 0;
	this->control = 0;
	this->controlT1 = 0;
	this->setpoint = 0;
	this->measurement = 0;
}

void PID::reset() {
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

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */
