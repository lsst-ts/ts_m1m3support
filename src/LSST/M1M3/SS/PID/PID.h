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

#ifndef PID_H_
#define PID_H_

#include <PIDParameters.h>
#include <M1M3SSPublisher.h>
#include <SAL_MTM1M3C.h>

namespace LSST {
namespace M1M3 {
namespace SS {

/**
 * Implements PID discrete time controller.
 *
 * See PID discussion at
 * [Confluence](https://confluence.lsstcorp.org/pages/viewpage.action?pageId=34209829)
 * for details. The [PID Implementation in
 * Software](https://confluence.lsstcorp.org/pages/viewpage.action?pageId=34209829&preview=/34209829/135102468/PID%20Implementation%20in%20Software%20v_2.pdf)
 * has details about the calculations.
 */
class PID {
public:
    /**
     * Constructs PID.
     *
     * @param id member ID. Index to fx,fy,fz and mx,my,mz
     * @param parameters PID parameters struct
     * @param publisher SAL interface
     */
    PID(int id, PIDParameters parameters);

    /**
     * Update PID parameters.
     */
    void updateParameters(PIDParameters parameters);
    void restoreInitialParameters();
    void resetPreviousValues();

    /**
     * Run PID calculations, produce output.
     */
    double process(double setpoint, double measurement);

    void publishTelemetry();

private:
    int _id;

    //* initial parameters passed in constructor
    PIDParameters _initialParameters;
    MTM1M3_logevent_pidInfoC* _pidInfo;
    MTM1M3_pidDataC* _pidData;

    void _calculateIntermediateValues();
    void _publishInfo();
};

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */

#endif /* PID_H_ */
