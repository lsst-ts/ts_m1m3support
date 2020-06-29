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

#ifndef POWERCONTROLLER_H_
#define POWERCONTROLLER_H_

#include <DataTypes.h>
#include <IFPGA.h>
#include <IExpansionFPGA.h>

struct MTM1M3_powerSupplyDataC;
struct MTM1M3_logevent_powerStatusC;
struct MTM1M3_logevent_powerSupplyStatusC;
struct MTM1M3_logevent_powerWarningC;

namespace LSST {
namespace M1M3 {
namespace SS {

struct SupportFPGAData;
class M1M3SSPublisher;
class SafetyController;

/*!
 * The class used to control and process power data.
 */
class PowerController {
public:
    /*!
     * Instantiates the power controller.
     * @param[in] publisher The publisher.
     * @param[in] safetyController The safety controller.
     */
    PowerController(M1M3SSPublisher* publisher, SafetyController* safetyController);

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

private:
    M1M3SSPublisher* _publisher;
    SafetyController* _safetyController;

    MTM1M3_powerSupplyDataC* _powerSupplyData;
    MTM1M3_logevent_powerStatusC* _powerStatus;
    MTM1M3_logevent_powerSupplyStatusC* _powerSupplyStatus;
    MTM1M3_logevent_powerWarningC* _powerWarning;

    uint64_t _lastPowerTimestamp;
};

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */

#endif /* POWERCONTROLLER_H_ */
