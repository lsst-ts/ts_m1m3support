/*
 * SimulatedFPGA.h
 *
 *  Created on: Nov 2, 2018
 *      Author: ccontaxis
 */

#ifndef LSST_M1M3_SS_FPGA_SIMULATEDFPGA_H_
#define LSST_M1M3_SS_FPGA_SIMULATEDFPGA_H_

#include <IFPGA.h>
#include <queue>
#include <ILCSubnetData.h>

#define RND_CNT 50

struct MTMount_ElevationC;
struct MTMount_AzimuthC;
struct MTM1M3_logevent_appliedCylinderForcesC;
struct MTM1M3_logevent_appliedHardpointStepsC;
struct MTM1M3_hardpointActuatorDataC;
struct MTM1M3_outerLoopDataC;

namespace LSST {
namespace M1M3 {
namespace SS {

class M1M3SSPublisher;
class ForceActuatorApplicationSettings;

class SimulatedFPGA : public IFPGA {
   private:
    M1M3SSPublisher* publisher;
    SupportFPGAData supportFPGAData;
    ForceActuatorApplicationSettings* forceActuatorApplicationSettings;
    MTMount_ElevationC* tmaElevation;
    MTMount_AzimuthC* tmaAzimuth;
    MTM1M3_logevent_appliedCylinderForcesC* appliedCylinderForces;
    MTM1M3_logevent_appliedHardpointStepsC* appliedHardpointSteps;
    MTM1M3_hardpointActuatorDataC* hardpointActuatorData;
    MTM1M3_outerLoopDataC* outerLoopData;
    int lastRequest;
    std::queue<uint16_t> u8Response;
    std::queue<uint16_t> u16Response;
    std::queue<uint16_t> subnetAResponse;
    std::queue<uint16_t> subnetBResponse;
    std::queue<uint16_t> subnetCResponse;
    std::queue<uint16_t> subnetDResponse;
    std::queue<uint16_t> subnetEResponse;

    std::queue<uint16_t> crcVector;
    void writeModbus(std::queue<uint16_t>* response, uint16_t data);
    void writeModbusCRC(std::queue<uint16_t>* response);
    uint16_t readModbus(uint16_t data);

    float rnd[RND_CNT];
    int rndIndex;
    float getRnd();

   public:
    SimulatedFPGA(M1M3SSPublisher* publisher, MTMount_ElevationC* tmaElevation, MTMount_AzimuthC* tmaAzimuth, ForceActuatorApplicationSettings* forceActuatorApplicationSettings);

    SupportFPGAData* getSupportFPGAData() { return &this->supportFPGAData; }

    int32_t initialize();
    int32_t open();
    int32_t close();
    int32_t finalize();

    bool isErrorCode(int32_t status);

    int32_t waitForOuterLoopClock(int32_t timeout);
    int32_t ackOuterLoopClock();

    int32_t waitForPPS(int32_t timeout);
    int32_t ackPPS();

    int32_t waitForModbusIRQ(int32_t subnet, int32_t timeout);
    int32_t ackModbusIRQ(int32_t subnet);

    void pullTelemetry();

    int32_t writeCommandFIFO(uint16_t* data, int32_t length, int32_t timeoutInMs);
    int32_t writeCommandFIFO(uint16_t data, int32_t timeoutInMs);
    int32_t writeRequestFIFO(uint16_t* data, int32_t length, int32_t timeoutInMs);
    int32_t writeRequestFIFO(uint16_t data, int32_t timeoutInMs);
    int32_t writeTimestampFIFO(uint64_t timestamp);
    int32_t readU8ResponseFIFO(uint8_t* data, int32_t length, int32_t timeoutInMs);
    int32_t readU16ResponseFIFO(uint16_t* data, int32_t length, int32_t timeoutInMs);
};

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */

#endif /* LSST_M1M3_SS_FPGA_SIMULATEDFPGA_H_ */
