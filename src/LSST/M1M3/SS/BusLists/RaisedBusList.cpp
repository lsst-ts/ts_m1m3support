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

#include <RaisedBusList.h>
#include <ILCSubnetData.h>
#include <ILCMessageFactory.h>
#include <M1M3SSPublisher.h>
#include <RoundRobin.h>
#include <SAL_MTM1M3C.h>
#include <cstring>
#include <spdlog/spdlog.h>

using namespace LSST::M1M3::SS;

RaisedBusList::RaisedBusList(ILCSubnetData* subnetData, ILCMessageFactory* ilcMessageFactory)
        : BusList(subnetData, ilcMessageFactory) {
    SPDLOG_DEBUG("RaisedBusList: RaisedBusList()");
    _outerLoopData = M1M3SSPublisher::instance().getOuterLoopData();
    _appliedCylinderForces = M1M3SSPublisher::instance().getAppliedCylinderForces();
    _hardpointActuatorData = M1M3SSPublisher::instance().getHardpointActuatorData();
    _forceInfo = M1M3SSPublisher::instance().getEventForceActuatorInfo();
}

void RaisedBusList::buildBuffer() {
    BusList::buildBuffer();
    SPDLOG_DEBUG("RaisedBusList: buildBuffer()");

    _lvdtSampleClock = 0;
    for (int subnetIndex = 0; subnetIndex < SUBNET_COUNT; subnetIndex++) {
        _setForceCommandIndex[subnetIndex] = -1;
        _moveStepCommandIndex[subnetIndex] = -1;
        _faStatusCommandIndex[subnetIndex] = -1;
        _roundRobinFAReportServerStatusIndex[subnetIndex] = 0;
    }
    for (int subnetIndex = 0; subnetIndex < SUBNET_COUNT; subnetIndex++) {
        startSubnet(subnetIndex);
        if (subnetData->getFACount(subnetIndex) > 0) {
            _setForceCommandIndex[subnetIndex] = buffer.getIndex();
            int32_t saaPrimary[16];
            int32_t daaPrimary[32];
            int32_t daaSecondary[32];
            memset(saaPrimary, 0, sizeof(saaPrimary));
            memset(daaPrimary, 0, sizeof(daaPrimary));
            memset(daaSecondary, 0, sizeof(daaSecondary));
            for (int faIndex = 0; faIndex < subnetData->getFACount(subnetIndex); faIndex++) {
                uint8_t address = subnetData->getFAIndex(subnetIndex, faIndex).Address;
                int32_t primaryDataIndex = subnetData->getFAIndex(subnetIndex, faIndex).DataIndex;
                int32_t secondaryDataIndex = subnetData->getFAIndex(subnetIndex, faIndex).SecondaryDataIndex;

                if (address <= 16) {
                    saaPrimary[address - 1] = _appliedCylinderForces->primaryCylinderForces[primaryDataIndex];
                } else {
                    daaPrimary[address - 17] =
                            _appliedCylinderForces->primaryCylinderForces[primaryDataIndex];
                    daaSecondary[address - 17] =
                            _appliedCylinderForces->secondaryCylinderForces[secondaryDataIndex];
                }
            }
            ilcMessageFactory->broadcastForceDemand(&buffer, _outerLoopData->broadcastCounter,
                                                    _outerLoopData->slewFlag, saaPrimary, daaPrimary,
                                                    daaSecondary);
            buffer.writeTimestamp();
            for (int faIndex = 0; faIndex < subnetData->getFACount(subnetIndex); faIndex++) {
                uint8_t address = subnetData->getFAIndex(subnetIndex, faIndex).Address;
                int32_t dataIndex = subnetData->getFAIndex(subnetIndex, faIndex).DataIndex;
                bool disabled = subnetData->getFAIndex(subnetIndex, faIndex).Disabled;
                if (!disabled) {
                    ilcMessageFactory->pneumaticForceStatus(&buffer, address);
                    expectedFAResponses[dataIndex] = 1;
                } else {
                    expectedFAResponses[dataIndex] = 0;
                }
            }
            int32_t statusIndex = _roundRobinFAReportServerStatusIndex[subnetIndex];
            while (subnetData->getFAIndex(subnetIndex, statusIndex).Disabled) {
                _roundRobinFAReportServerStatusIndex[subnetIndex] =
                        RoundRobin::Inc(statusIndex, subnetData->getFACount(subnetIndex));
                statusIndex = _roundRobinFAReportServerStatusIndex[subnetIndex];
            }
            uint8_t address = subnetData->getFAIndex(subnetIndex, statusIndex).Address;
            int32_t dataIndex = subnetData->getFAIndex(subnetIndex, statusIndex).DataIndex;
            _faStatusCommandIndex[subnetIndex] = buffer.getIndex();
            ilcMessageFactory->reportDCAPressure(&buffer, address);
            ilcMessageFactory->reportServerStatus(&buffer, address);
            expectedFAResponses[dataIndex] = 3;
        }
        if (subnetData->getHPCount(subnetIndex) > 0) {
            _moveStepCommandIndex[subnetIndex] = buffer.getIndex();
            int8_t steps[78];
            for (int hpIndex = 0; hpIndex < subnetData->getHPCount(subnetIndex); hpIndex++) {
                uint8_t address = subnetData->getHPIndex(subnetIndex, hpIndex).Address;
                int32_t dataIndex = subnetData->getHPIndex(subnetIndex, hpIndex).DataIndex;
                // Steps are swapped because negative steps extend and positive steps retract
                // This doesn't match what most people would expect so we are swapping it
                steps[address - 1] = -_hardpointActuatorData->stepsCommanded[dataIndex];
            }
            ilcMessageFactory->broadcastStepMotor(&buffer, _outerLoopData->broadcastCounter, steps);
            buffer.writeTimestamp();
            for (int hpIndex = 0; hpIndex < subnetData->getHPCount(subnetIndex); hpIndex++) {
                uint8_t address = subnetData->getHPIndex(subnetIndex, hpIndex).Address;
                int32_t dataIndex = subnetData->getHPIndex(subnetIndex, hpIndex).DataIndex;
                bool disabled = subnetData->getHPIndex(subnetIndex, hpIndex).Disabled;
                if (!disabled) {
                    ilcMessageFactory->electromechanicalForceAndStatus(&buffer, address);
                    ilcMessageFactory->reportServerStatus(&buffer, address);
                    expectedHPResponses[dataIndex] = 2;
                }
            }
        }
        for (int hmIndex = 0; hmIndex < subnetData->getHMCount(subnetIndex); hmIndex++) {
            uint8_t address = subnetData->getHMIndex(subnetIndex, hmIndex).Address;
            int32_t dataIndex = subnetData->getHMIndex(subnetIndex, hmIndex).DataIndex;
            bool disabled = subnetData->getHMIndex(subnetIndex, hmIndex).Disabled;
            if (!disabled) {
                ilcMessageFactory->reportDCAPressure(&buffer, address);
                ilcMessageFactory->reportDCAStatus(&buffer, address);
                ilcMessageFactory->reportServerStatus(&buffer, address);
                expectedHMResponses[dataIndex] = 3;
            }
        }
        if (subnetData->getHMCount(subnetIndex) > 0) {
            _hmLVDTCommandIndex[subnetIndex] = buffer.getIndex();
            for (int hmIndex = 0; hmIndex < subnetData->getHMCount(subnetIndex); hmIndex++) {
                uint8_t address = subnetData->getHMIndex(subnetIndex, hmIndex).Address;
                bool disabled = subnetData->getHMIndex(subnetIndex, hmIndex).Disabled;
                if (!disabled) {
                    ilcMessageFactory->nopReportLVDT(&buffer, address);
                }
            }
        }
        endSubnet();
    }
    buffer.setLength(buffer.getIndex());
}

void RaisedBusList::update() {
    _outerLoopData->broadcastCounter = RoundRobin::BroadcastCounter(_outerLoopData->broadcastCounter);

    for (int subnetIndex = 0; subnetIndex < SUBNET_COUNT; subnetIndex++) {
        if (subnetData->getFACount(subnetIndex) > 0) {
            int32_t saaPrimary[16];
            int32_t daaPrimary[32];
            int32_t daaSecondary[32];
            memset(saaPrimary, 0, sizeof(saaPrimary));
            memset(daaPrimary, 0, sizeof(daaPrimary));
            memset(daaSecondary, 0, sizeof(daaSecondary));
            for (int faIndex = 0; faIndex < subnetData->getFACount(subnetIndex); faIndex++) {
                uint8_t address = subnetData->getFAIndex(subnetIndex, faIndex).Address;
                int32_t primaryDataIndex = subnetData->getFAIndex(subnetIndex, faIndex).DataIndex;
                int32_t secondaryDataIndex = subnetData->getFAIndex(subnetIndex, faIndex).SecondaryDataIndex;

                if (address <= 16) {
                    saaPrimary[address - 1] = _appliedCylinderForces->primaryCylinderForces[primaryDataIndex];
                } else {
                    daaPrimary[address - 17] =
                            _appliedCylinderForces->primaryCylinderForces[primaryDataIndex];
                    daaSecondary[address - 17] =
                            _appliedCylinderForces->secondaryCylinderForces[secondaryDataIndex];
                }
            }
            buffer.setIndex(_setForceCommandIndex[subnetIndex]);
            ilcMessageFactory->broadcastForceDemand(&buffer, _outerLoopData->broadcastCounter,
                                                    _outerLoopData->slewFlag, saaPrimary, daaPrimary,
                                                    daaSecondary);

            int32_t statusIndex = _roundRobinFAReportServerStatusIndex[subnetIndex];
            int32_t dataIndex = subnetData->getFAIndex(subnetIndex, statusIndex).DataIndex;
            // revert back previous data index; this needs to be enabled, as
            // only enabled FA are considered
            expectedFAResponses[dataIndex] = 1;
            _roundRobinFAReportServerStatusIndex[subnetIndex] =
                    RoundRobin::Inc(statusIndex, subnetData->getFACount(subnetIndex));
            statusIndex = _roundRobinFAReportServerStatusIndex[subnetIndex];
            while (subnetData->getFAIndex(subnetIndex, statusIndex).Disabled) {
                _roundRobinFAReportServerStatusIndex[subnetIndex] =
                        RoundRobin::Inc(statusIndex, subnetData->getFACount(subnetIndex));
                statusIndex = _roundRobinFAReportServerStatusIndex[subnetIndex];
            }
            uint8_t address = subnetData->getFAIndex(subnetIndex, statusIndex).Address;
            dataIndex = subnetData->getFAIndex(subnetIndex, statusIndex).DataIndex;

            buffer.setIndex(_faStatusCommandIndex[subnetIndex]);
            ilcMessageFactory->reportDCAPressure(&buffer, address);
            ilcMessageFactory->reportServerStatus(&buffer, address);
            expectedFAResponses[dataIndex] = 3;
        }
        if (subnetData->getHPCount(subnetIndex) > 0) {
            int8_t steps[78];
            for (int hpIndex = 0; hpIndex < subnetData->getHPCount(subnetIndex); hpIndex++) {
                uint8_t address = subnetData->getHPIndex(subnetIndex, hpIndex).Address;
                int32_t dataIndex = subnetData->getHPIndex(subnetIndex, hpIndex).DataIndex;
                // Steps are swapped because negative steps extend and positive steps retract
                // This doesn't match what most people would expect so we are swapping it
                steps[address - 1] = -_hardpointActuatorData->stepsCommanded[dataIndex];
            }
            buffer.setIndex(_moveStepCommandIndex[subnetIndex]);
            ilcMessageFactory->broadcastStepMotor(&buffer, _outerLoopData->broadcastCounter, steps);
        }
        if (subnetData->getHMCount(subnetIndex) > 0) {
            buffer.setIndex(_hmLVDTCommandIndex[subnetIndex]);
            for (int hmIndex = 0; hmIndex < subnetData->getHMCount(subnetIndex); hmIndex++) {
                uint8_t address = subnetData->getHMIndex(subnetIndex, hmIndex).Address;
                int32_t dataIndex = subnetData->getHMIndex(subnetIndex, hmIndex).DataIndex;
                bool disabled = subnetData->getHMIndex(subnetIndex, hmIndex).Disabled;
                if (!disabled) {
                    if (_lvdtSampleClock == 0) {
                        ilcMessageFactory->reportLVDT(&buffer, address);
                        expectedHMResponses[dataIndex] = 4;
                    } else {
                        ilcMessageFactory->nopReportLVDT(&buffer, address);
                        expectedHMResponses[dataIndex] = 3;
                    }
                }
            }
            _lvdtSampleClock--;
            if (_lvdtSampleClock < 0) {
                _lvdtSampleClock = 4;
            }
        }
    }
}
