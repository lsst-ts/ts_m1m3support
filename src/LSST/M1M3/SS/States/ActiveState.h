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

#ifndef ACTIVESTATE_H_
#define ACTIVESTATE_H_

#include <EnabledState.h>

namespace LSST {
namespace M1M3 {
namespace SS {

/**
 * Active State. Mirror is raised, supported by force actuators. Mirror can be
 * lowered with Lower M1M3 Command. The controller can be switched to
 * engineering mode with Enter Engineering command.
 */
class ActiveState : public EnabledState {
public:
    ActiveState();

    virtual States::Type update(UpdateCommand* command) override;
    virtual States::Type enterEngineering(EnterEngineeringCommand* command) override;
    virtual States::Type lowerM1M3(LowerM1M3Command* command) override;
    virtual States::Type enableHardpointCorrections(EnableHardpointCorrectionsCommand* command) override;
    virtual States::Type disableHardpointCorrections(DisableHardpointCorrectionsCommand* command) override;
};

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */

#endif /* ACTIVESTATE_H_ */
