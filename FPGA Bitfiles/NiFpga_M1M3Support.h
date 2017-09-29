/*
 * Generated with the FPGA Interface C API Generator 16.0.0
 * for NI-RIO 16.0.0 or later.
 */

#ifndef __NiFpga_M1M3Support_h__
#define __NiFpga_M1M3Support_h__

#ifndef NiFpga_Version
   #define NiFpga_Version 1600
#endif

#include "NiFpga.h"

/**
 * The filename of the FPGA bitfile.
 *
 * This is a #define to allow for string literal concatenation. For example:
 *
 *    static const char* const Bitfile = "C:\\" NiFpga_M1M3Support_Bitfile;
 */
#define NiFpga_M1M3Support_Bitfile "NiFpga_M1M3Support.lvbitx"

/**
 * The signature of the FPGA bitfile.
 */
static const char* const NiFpga_M1M3Support_Signature = "FD0940FB42D25D222CBA4910208204B6";

typedef enum
{
   NiFpga_M1M3Support_IndicatorU64_Data = 0x18000,
} NiFpga_M1M3Support_IndicatorU64;

typedef enum
{
   NiFpga_M1M3Support_ControlBool_ModbusTrigger = 0x18006,
} NiFpga_M1M3Support_ControlBool;

typedef enum
{
   NiFpga_M1M3Support_TargetToHostFifoU8_RS232RxFIFO = 2,
} NiFpga_M1M3Support_TargetToHostFifoU8;

typedef enum
{
   NiFpga_M1M3Support_TargetToHostFifoU16_ModbusRxDataFIFO = 5,
} NiFpga_M1M3Support_TargetToHostFifoU16;

typedef enum
{
   NiFpga_M1M3Support_TargetToHostFifoU64_HealthAndStatusDataFIFO = 6,
   NiFpga_M1M3Support_TargetToHostFifoU64_ModbusTxTimestampFIFO = 3,
} NiFpga_M1M3Support_TargetToHostFifoU64;

typedef enum
{
   NiFpga_M1M3Support_HostToTargetFifoU8_RS232TxFIFO = 1,
} NiFpga_M1M3Support_HostToTargetFifoU8;

typedef enum
{
   NiFpga_M1M3Support_HostToTargetFifoU16_ModbusTxDataFIFO = 4,
} NiFpga_M1M3Support_HostToTargetFifoU16;

typedef enum
{
   NiFpga_M1M3Support_HostToTargetFifoU64_HealthAndStatusControlFIFO = 7,
   NiFpga_M1M3Support_HostToTargetFifoU64_TimestampControlFIFO = 0,
} NiFpga_M1M3Support_HostToTargetFifoU64;

#endif
