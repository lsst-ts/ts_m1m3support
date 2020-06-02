#ifndef MODBUSBUFFER_H_
#define MODBUSBUFFER_H_

#include <DataTypes.h>
#include <string>

namespace LSST {
namespace M1M3 {
namespace SS {

/**
 * Utility class for Modbus buffer management. Provides function to write and
 * read cRIO FIFO (FPGA) Modbus buffers.
 *
 * 8bit data are stored as 16bit values. Real data are left shifted by 1. Last
 * bit (0, transmitted first) is start bit, always 1 for ILC communication.
 */
class ModbusBuffer {
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

    /**
     * Set empty buffer.
     */
    void reset();
    bool endOfBuffer();
    bool endOfFrame();

    uint16_t calculateCRC(int32_t length);

    uint16_t readLength();
    int32_t readI32();
    uint8_t readU8();
    uint16_t readU16();
    uint32_t readU32();
    uint64_t readU48();
    float readSGL();
    std::string readString(int32_t length);
    uint16_t readCRC();
    double readTimestamp();

    /**
     * Reads instruction byte from FPGA FIFO.
     *
     * @param instruction instruction from Response FIFO.
     *
     * @return byte written by the instruction. Start bit is removed.
     */
    static inline uint8_t readInstructionByte(uint16_t instruction) { return (uint8_t)((instruction >> 1) & 0xFF); }

    void readEndOfFrame();

    void writeSubnet(uint8_t data);
    void writeLength(uint16_t data);
    void writeI8(int8_t data);
    void writeI16(int16_t data);
    void writeI24(int32_t data);
    void writeI32(int32_t data);
    void writeU8(uint8_t data);
    void writeU16(uint16_t data);
    void writeU32(uint32_t data);
    void writeSGL(float data);
    void writeCRC(int32_t length);

    /**
     * Return data item to write to buffer.
     *
     * @param data data to write.
     *
     * @return 16bit for command queue.
     */
    static inline uint16_t writeByteInstruction(uint8_t data) { return (((uint16_t)data) << 1) | 0x1200; };

    void writeDelay(uint32_t delayMicros);
    void writeEndOfFrame();
    void writeSoftwareTrigger();
    void writeTimestamp();
    void writeTriggerIRQ();
    void writeWaitForRx(uint32_t timeoutMicros);

private:
    uint16_t buffer[5120];
    uint8_t floatPointBuffer[8];
    uint8_t stringBuffer[256];
    int32_t index;
    int32_t length;
};

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */

#endif /* MODBUSBUFFER_H_ */
