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

#ifndef AUTOMATICOPERATIONSCONTROLLER_H_
#define AUTOMATICOPERATIONSCONTROLLER_H_

namespace LSST {
namespace M1M3 {
namespace SS {

class PositionController;
class ForceController;
class SafetyController;
class M1M3SSPublisher;
class PowerController;

class AutomaticOperationsController {
public:
    AutomaticOperationsController(PositionController* positionController, ForceController* forceController,
                                  SafetyController* safetyController, M1M3SSPublisher* publisher,
                                  PowerController* powerController);

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
    M1M3SSPublisher* _publisher;
    PowerController* _powerController;
    double _cachedTimestamp;
    bool _bypassMoveToReference;
};

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */

#endif /* AUTOMATICOPERATIONSCONTROLLER_H_ */
