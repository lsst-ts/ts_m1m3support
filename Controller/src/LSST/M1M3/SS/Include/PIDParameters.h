/*
 * PIDParameters.h
 *
 *  Created on: Feb 13, 2018
 *      Author: ccontaxis
 */

#ifndef PIDPARAMETERS_H_
#define PIDPARAMETERS_H_

namespace LSST {
namespace M1M3 {
namespace SS {

struct PIDParameters {
    double Timestep;
    double P;
    double I;
    double D;
    double N;
};

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */

#endif /* PIDPARAMETERS_H_ */
