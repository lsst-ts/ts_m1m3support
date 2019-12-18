/*
 * PowerController.h
 *
 *  Created on: Dec 7, 2017
 *      Author: ccontaxis
 */

#ifndef POWERCONTROLLER_H_
#define POWERCONTROLLER_H_

#include <DataTypes.h>

struct MTM1M3_powerDataC;
struct MTM1M3_logevent_powerStatusC;
struct MTM1M3_logevent_powerWarningC;

namespace LSST {
namespace M1M3 {
namespace SS {

class IFPGA;
struct SupportFPGAData;
class IExpansionFPGA;
class M1M3SSPublisher;
class SafetyController;

/*!
 * The class used to control and process power data.
 */
class PowerController {
private:
    IFPGA* fpga;
    SupportFPGAData* fpgaData;
    IExpansionFPGA* expansionFPGA;
    M1M3SSPublisher* publisher;
    SafetyController* safetyController;

    MTM1M3_powerDataC* powerData;
    MTM1M3_logevent_powerStatusC* powerStatus;
    MTM1M3_logevent_powerWarningC* powerWarning;

    uint64_t lastPowerTimestamp;

public:
    /*!
	 * Instantiates the power controller.
	 * @param[in] fpga The fpga.
	 * @param[in] expansionFPGA The expansion fpga.
	 * @param[in] publisher The publisher.
	 * @param[in] safetyController The safety controller.
	 */
    PowerController(IFPGA* fpga, IExpansionFPGA* expansionFPGA, M1M3SSPublisher* publisher, SafetyController* safetyController);

    /*!
	 * Processes currently available power data and publish it.
	 */
    void processData();

    /*!
	 * Turns all power networks on or off.
	 * @param[in] on True to turn on.
	 */
    void setBothPowerNetworks(bool on);

    /*!
	 * Turns all normal power networks on or off.
	 * @param[in] on True to turn on.
	 */
    void setAllPowerNetworks(bool on);
    /*!
	 * Turns normal power network A on or off.
	 * @param[in] on True to turn on.
	 */
    void setPowerNetworkA(bool on);
    /*!
	 * Turns normal power network B on or off.
	 * @param[in] on True to turn on.
	 */
    void setPowerNetworkB(bool on);
    /*!
	 * Turns normal power network C on or off.
	 * @param[in] on True to turn on.
	 */
    void setPowerNetworkC(bool on);
    /*!
	 * Turns normal power network D on or off.
	 * @param[in] on True to turn on.
	 */
    void setPowerNetworkD(bool on);

    /*!
	 * Turns all aux power networks on or off.
	 * @param[in] on True to turn on.
	 */
    void setAllAuxPowerNetworks(bool on);
    /*!
	 * Turns aux power network A on or off.
	 * @param[in] on True to turn on.
	 */
    void setAuxPowerNetworkA(bool on);
    /*!
	 * Turns aux power network B on or off.
	 * @param[in] on True to turn on.
	 */
    void setAuxPowerNetworkB(bool on);
    /*!
	 * Turns aux power network C on or off.
	 * @param[in] on True to turn on.
	 */
    void setAuxPowerNetworkC(bool on);
    /*!
	 * Turns aux power network D on or off.
	 * @param[in] on True to turn on.
	 */
    void setAuxPowerNetworkD(bool on);
};

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */

#endif /* POWERCONTROLLER_H_ */
