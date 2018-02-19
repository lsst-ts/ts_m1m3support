/*
 * PID.h
 *
 *  Created on: Feb 13, 2018
 *      Author: ccontaxis
 */

#ifndef PID_H_
#define PID_H_

#include <PIDParameters.h>

struct m1m3_logevent_PIDInfoC;
struct m1m3_PIDDataC;

namespace LSST {
namespace M1M3 {
namespace SS {

class M1M3SSPublisher;

class PID {
private:
	int id;
	PIDParameters initialParameters;
	M1M3SSPublisher* publisher;
	m1m3_logevent_PIDInfoC* pidInfo;
	m1m3_PIDDataC* pidData;
	PIDParameters parameters;
	double error;
	double errorT1;
	double errorT2;
	double control;
	double controlT1;
	double controlT2;
	double a;
	double b;
	double c;
	double d;
	double e;
	double setpoint;
	double measurement;

public:
	PID(int id, PIDParameters parameters, M1M3SSPublisher* publisher);

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
