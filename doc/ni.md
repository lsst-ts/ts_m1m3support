## NI-PC interaction

NI cRIO is running FPGA code programmed in LabVIEW and C++ binary code. FPGA
code handles details of low-level modbus communication. C++ code handles
communication over SAL with TCS, and communicates with FPGA with a command FIFO
- see writeCommandFIFO and readCommandFIFO calls:

```cpp
bool FirmwareUpdate::ClearFaults(int subnet, int address) {
        this->desiredState = 3;
        ModbusBuffer buffer;
        this->SetupBuffer(&buffer, subnet);
        buffer.writeU8((uint8_t)address);
        buffer.writeU8(65);
        buffer.writeU8(0);
        buffer.writeU8(5);
        buffer.writeCRC(4);
        buffer.writeEndOfFrame();
        buffer.writeWaitForRx(10000);
        this->EndBuffer(&buffer);
        this->fpga->writeCommandFIFO(buffer.getBuffer(), buffer.getLength(), 0);
        return this->ProcessResponse(&buffer, subnet);
}
```

The command is processed inside FPGA, put into command queue for serial/modbus
port, send over the serial line to Inner Loop Controllers (ILC), response is
read, added to response queue, added to command response queue and finally
readout.
