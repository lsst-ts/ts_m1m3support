/*
 * This file is part of LSST M1M3 support system package.
 *
 * Developed for the Vera C. Rubin Telescope and Site System.
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

#ifndef __ILCWarnign_h__
#define __ILCWarnign_h__

#include <map>

#include <SAL_MTM1M3.h>

#include <cRIO/DataTypes.h>
#include <cRIO/Singleton.h>

namespace LSST {
namespace M1M3 {
namespace SS {

class ILCWarning : public MTM1M3_logevent_ilcWarningC, public cRIO::Singleton<ILCWarning> {
public:
    ILCWarning(token);

    void warnResponseTimeout(double _timestamp, int32_t _actuator_id, bool _active);
    void warnInvalidCRC(double _timestamp, bool _active);
    void warnIllegalFunction(double _timestamp, int32_t _actuator_id, bool _active);
    void warnIllegalDataValue(double _timestamp, int32_t _actuator_id, bool _active);
    void warnInvalidLength(double _timestamp, int32_t _actuator_id, bool _active);
    void warnUnknownSubnet(double _timestamp, bool _active);
    void warnUnknownAddress(double _timestamp, int32_t _actuator_id, bool _active);
    void warnUnknownFunction(double _timestamp, int32_t _actuator_id, bool _active);
    void warnUnknownProblem(double _timestamp, int32_t _actuator_id, bool _active);

protected:
    bool ignore_warning(std::map<int, double> &_timeouts, double _timestamp, int _actuator_id, bool _active);

private:
    std::map<int, double> _last_response_timeout;
    std::map<int, double> _last_illegal_function;
    std::map<int, double> _last_illegal_date_value;
    std::map<int, double> _last_invalid_length;
    std::map<int, double> _last_unknown_subnet;
    std::map<int, double> _last_unknown_address;
    std::map<int, double> _last_unknown_function;
    std::map<int, double> _last_unknown_problem;

    float ignore_time;
};

}  // namespace SS
}  // namespace M1M3
}  // namespace LSST

#endif /* __ILCWarnign_h__ */
