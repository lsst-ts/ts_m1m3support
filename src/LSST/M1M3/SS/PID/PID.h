#ifndef PID_H_
#define PID_H_

#include <PIDParameters.h>
#include <M1M3SSPublisher.h>
#include <SAL_MTM1M3C.h>

namespace LSST {
namespace M1M3 {
namespace SS {

class M1M3SSPublisher;

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
    PID(int id, PIDParameters parameters, M1M3SSPublisher* publisher);

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
    M1M3SSPublisher* _publisher;
    MTM1M3_logevent_pidInfoC* _pidInfo;
    MTM1M3_pidDataC* _pidData;

    void _calculateIntermediateValues();
    void _publishInfo();
};

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */

#endif /* PID_H_ */
