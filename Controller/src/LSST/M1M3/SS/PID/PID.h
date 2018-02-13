/*
 * PID.h
 *
 *  Created on: Feb 13, 2018
 *      Author: ccontaxis
 */

#ifndef PID_H_
#define PID_H_

#include <PIDParameters.h>

namespace LSST {
namespace M1M3 {
namespace SS {

class PID {
private:
	int id;
	PIDParameters initialParameters;
	PIDParameters parameters;
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
	PID(int id, double timestep, double p, double i, double d, double n);
	PID(int id, PIDParameters parameters);

	void updateParameters(PIDParameters parameters);
	void restoreInitialParameters();
	void resetPreviousValues();

	double process(double setpoint, double measurement);

	void publishTelemetry();

private:
	void calculateIntermediateValues();

	void publishInfo();
};

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */

#endif /* PID_H_ */
