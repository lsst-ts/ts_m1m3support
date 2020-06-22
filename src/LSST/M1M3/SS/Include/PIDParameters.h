/*
 * This file is part of LSST M1M3 support system package.
 *
 * Developed for the LSST Data Management System.
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
