#ifndef MODBUSBUFFER_H_
#define MODBUSBUFFER_H_

#include <DataTypes.h>
#include <string>

namespace LSST {
namespace M1M3 {
namespace SS {

class ModbusBuffer {
private:
    uint16_t buffer[5120];
    uint8_t floatPointBuffer[8];
    uint8_t stringBuffer[256];
    int32_t index;
    int32_t length;

public:
    ModbusBuffer();
    virtual ~ModbusBuffer();

    int32_t getIndex();
    void setIndex(int32_t index);
    void incIndex(int32_t inc);
    void skipToNextFrame();
    uint16_t* getBuffer();
    void set(int32_t index, uint16_t data);
    void setLength(int32_t length);
    int32_t getLength();

    void reset();
    bool endOfBuffer();
    bool endOfFrame();

    uint16_t calculateCRC(int32_t length);

    uint8_t readSubnet();
    uint16_t readLength();
    int8_t readI8();
    int16_t readI16();
    int32_t readI32();
    int64_t readI64();
    uint8_t readU8();
    uint16_t readU16();
    uint32_t readU32();
    uint64_t readU48();
    uint64_t readU64();
    float readSGL();
    double readDBL();
    std::string readString(int32_t length);
    uint16_t readCRC();
    double readTimestamp();
    uint8_t readInstructionByte(uint16_t instruction);
    uint8_t readTimestampByte(uint16_t instruction);
    void readEndOfFrame();

    void writeSubnet(uint8_t data);
    void writeLength(uint16_t data);
    void writeI8(int8_t data);
    void writeI16(int16_t data);
    void writeI24(int32_t data);
    void writeI32(int32_t data);
    void writeI64(int64_t data);
    void writeU8(uint8_t data);
    void writeU16(uint16_t data);
    void writeU32(uint32_t data);
    void writeU64(uint64_t data);
    void writeSGL(float data);
    void writeDBL(double data);
    void writeCRC(int32_t length);
    uint16_t writeByteInstruction(uint8_t data);
    void writeDelay(uint32_t delayMicros);
    void writeEndOfFrame();
    void writeSoftwareTrigger();
    void writeTimestamp();
    void writeTriggerIRQ();
    void writeWaitForRx(uint32_t timeoutMicros);
};

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */

#endif /* MODBUSBUFFER_H_ */
