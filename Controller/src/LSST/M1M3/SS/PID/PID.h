/*
 * PID.h
 *
 *  Created on: Feb 13, 2018
 *      Author: ccontaxis
 */

#ifndef PID_H_
#define PID_H_

namespace LSST {
namespace M1M3 {
namespace SS {

class PID {
private:
	double p;
	double i;
	double d;
	double timestep;
	double error;
	double errorT1;
	double errorT2;
	double control;
	double controlT1;
	double a;
	double b;
	double c;
	double setpoint;
	double measurement;

public:
	PID(double p, double i, double d, double timestep);

	void reset();

	double process(double setpoint, double measurement);
};

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */

#endif /* PID_H_ */
