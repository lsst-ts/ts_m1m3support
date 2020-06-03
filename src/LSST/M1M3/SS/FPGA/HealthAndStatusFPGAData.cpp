#include "HealthAndStatusFPGAData.h"

#include "NiStatus.h"

namespace LSST {
namespace M1M3 {
namespace SS {

ModbusPort::ModbusPort()
        : errorFlag(0), txBytes(0), txFrames(0), rxBytes(0), rxFrames(0), instructionCount(0) {}

void ModbusPort::fillFromFIFO(uint64_t *buffer) {
    errorFlag = buffer[0];
    txBytes = buffer[1];
    txFrames = buffer[2];
    rxBytes = buffer[3];
    rxFrames = buffer[4];
    instructionCount = buffer[5];
}

void HealthAndStatusFPGAData::refresh(uint64_t buffer[PORTNUM * PORT_COUNTNUM]) {
    for (int i = 0; i < PORTNUM; i++) {
        ports[i].fillFromFIFO(buffer + i * PORT_COUNTNUM);
    }
}

}  // namespace SS
}  // namespace M1M3
}  // namespace LSST
