#ifndef LSST_M1M3_SS_FPGA_SIMULATEDFPGA_H_
#define LSST_M1M3_SS_FPGA_SIMULATEDFPGA_H_

#include <IFPGA.h>
#include <queue>
#include <ILCSubnetData.h>

#include <SAL_MTMountC.h>

#define RND_CNT 50

struct MTMount_AltC;
struct MTMount_AzC;
struct MTM1M3_logevent_appliedCylinderForcesC;
struct MTM1M3_logevent_appliedHardpointStepsC;
struct MTM1M3_hardpointActuatorDataC;
struct MTM1M3_outerLoopDataC;

namespace LSST {
namespace M1M3 {
namespace SS {

class M1M3SSPublisher;
class ForceActuatorApplicationSettings;

/**
 * FPGA simulator. Simulates MODBUS communication with devices.
 */
class SimulatedFPGA : public IFPGA {
public:
    SimulatedFPGA();

    void setPublisher(M1M3SSPublisher* publisher);
    void setForceActuatorApplicationSettings(
            ForceActuatorApplicationSettings* forceActuatorApplicationSettings);

    int32_t initialize() override;
    int32_t open() override;
    int32_t close() override;
    int32_t finalize() override;

    bool isErrorCode(int32_t status) override;

    int32_t waitForOuterLoopClock(int32_t timeout) override;
    int32_t ackOuterLoopClock() override;

    int32_t waitForPPS(int32_t timeout) override;
    int32_t ackPPS() override;

    int32_t waitForModbusIRQ(int32_t subnet, int32_t timeout) override;
    int32_t ackModbusIRQ(int32_t subnet) override;

    void pullTelemetry() override;
    void pullHealthAndStatus() override;

    int32_t writeCommandFIFO(uint16_t* data, int32_t length, int32_t timeoutInMs) override;
    int32_t writeCommandFIFO(uint16_t data, int32_t timeoutInMs) override;
    void writeRequestFIFO(uint16_t* data, int32_t length, int32_t timeoutInMs) override;
    void writeRequestFIFO(uint16_t data, int32_t timeoutInMs) override;
    int32_t writeTimestampFIFO(uint64_t timestamp) override;
    int32_t readU8ResponseFIFO(uint8_t* data, int32_t length, int32_t timeoutInMs) override;
    void readU16ResponseFIFO(uint16_t* data, int32_t length, int32_t timeoutInMs) override;

    void writeHealthAndStatusFIFO(uint16_t request, uint16_t param = 0) override;
    int32_t readHealthAndStatusFIFO(uint64_t* data, int32_t length, int32_t timeoutInMs = 10) override;

private:
    M1M3SSPublisher* publisher;
    ForceActuatorApplicationSettings* forceActuatorApplicationSettings;
    MTMount_AzimuthC tmaAzimuth;
    MTMount_ElevationC tmaElevation;
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
};

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */

#endif /* LSST_M1M3_SS_FPGA_SIMULATEDFPGA_H_ */
