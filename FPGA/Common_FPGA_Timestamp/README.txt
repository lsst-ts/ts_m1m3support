Usage for timestamping:
1. Generate a register for storing the timestamp. Specify a data type of U64. Specify the write to never arbitrate.
2. Generate a target to host - DMA FIFO for updating the timestamp. Specify a data type of U64. Specify a number of elements of 2. Specify never arbitrate for read.
3. Generate a 5MHz derived clock for timing.
4. Drop Timestamp.vi onto your main VI. Setup the timestamp settings using the resources defined above. Specify IterationsPerMicrosecond of 4 (at 5MHz, every fifth iteration will increment the clock).

Notes:
- To read the timestamp add the timestamp register generated in step 1 to any code you require timestamp data from.
- To update the timestamp write to the FIFO for updating the timestamp generated in step 2.
- It is recommended that you update the FPGA clock periodically to correct for drift.
- To convert a timestamp use the Timestamp.convertRawTimestamp vi, it doesn't require error fields or an actual Timestamp class.

Timestamp Format:
Bits   Description
19-00  Microseconds
63-20  Seconds

Converting RawTimestamp to TimestampInSeconds
TimestampInSeconds = (double)((RawTimestamp & 0xFFFFFFFFFFF00000) >> 20) + (double)((RawTimestamp & 0x00000000000FFFFF) / 1000000.0)
