#ifndef PIDPARAMETERS_H_
#define PIDPARAMETERS_H_

namespace LSST {
namespace M1M3 {
namespace SS {

/**
 * Parameters for PID calculations. Used in PID.
 */
struct PIDParameters {
    //* Size of step (seconds?)
    double Timestep;
    //* Proportional gain (unitless)
    double P;
    //* Integral gain (unitless)
    double I;
    //* Derivative gain (unitless)
    double D;
    //*
    double N;
};

} /* namespace SS */
}  // namespace M1M3
}  // namespace LSST

#endif /* PIDPARAMETERS_H_ */
