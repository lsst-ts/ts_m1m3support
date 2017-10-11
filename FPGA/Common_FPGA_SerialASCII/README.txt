Usage for each sensor:
1. Generate a target-scoped FIFO (or host to target) for Tx data. Specify a data type of U8.
2. Generate a target-scoped FIFO (or target to host) for Rx data. Specify a data type of U8.

Command Structure:
[LENGTH][DATA]
[LENGTH] = U16 indicating the length of [DATA]
[DATA] = ASCII_CHAR[] as U8[] of the data to send on the serial port

Notes:
- Use DMA FIFOs if you don't need to multiplex data transfers due to DMA FIFO limits
- Use target-scoped FIFOs if you need to multiplex the data transfers due to DMA FIFO limits
- Make sure the protocol has a terminator otherwise the application