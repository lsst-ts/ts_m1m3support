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

#ifndef MIRRORCONTROLLER_H_
#define MIRRORCONTROLLER_H_

#include <PositionController.h>
#include <ForceController.h>
#include <SafetyController.h>
#include <PowerController.h>

namespace LSST {
namespace M1M3 {
namespace SS {

/**
 * Controls mirror raising and lowering operations. The operations are executed
 * as command. Command transition system to some "in-progress" state. The newly
 * set system state calls in a loop method the MirrorController to perform
 * operations requested.
 */
class MirrorController {
public:
    MirrorController(PositionController* positionController, ForceController* forceController,
                     SafetyController* safetyController, PowerController* powerController);

    void startRaiseOperation(bool bypassMoveToReference);
    void tryIncrementingSupportPercentage();
    bool checkRaiseOperationComplete();
    void completeRaiseOperation();
    bool checkRaiseOperationTimeout();
    void timeoutRaiseOperation();

    void abortRaiseM1M3();

    void startLowerOperation();
    void tryDecrementSupportPercentage();
    bool checkLowerOperationComplete();
    void completeLowerOperation();
    bool checkLowerOperationTimeout();
    void timeoutLowerOperation();

    void uncontrolledLowerOperation();

private:
    PositionController* _positionController;
    ForceController* _forceController;
    SafetyController* _safetyController;
    PowerController* _powerController;
    double _cachedTimestamp;
    bool _bypassMoveToReference;
};

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */

#endif /* MIRRORCONTROLLER_H_ */
