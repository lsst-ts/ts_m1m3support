/*
 * Generated with the FPGA Interface C API Generator 19.0
 * for NI-RIO 19.0 or later.
 */
#ifndef __NiFpga_ts_M1M3SupportFPGA_h__
#define __NiFpga_ts_M1M3SupportFPGA_h__

#ifndef NiFpga_Version
   #define NiFpga_Version 190
#endif

#include "NiFpga.h"

/**
 * The filename of the FPGA bitfile.
 *
 * This is a #define to allow for string literal concatenation. For example:
 *
 *    static const char* const Bitfile = "C:\\" NiFpga_ts_M1M3SupportFPGA_Bitfile;
 */
#define NiFpga_ts_M1M3SupportFPGA_Bitfile "NiFpga_ts_M1M3SupportFPGA.lvbitx"

/**
 * The signature of the FPGA bitfile.
 */
static const char* const NiFpga_ts_M1M3SupportFPGA_Signature = "21F62B38B1E2C12E19E4B8337C8A93D1";

#if NiFpga_Cpp
extern "C"
{
#endif

typedef enum
{
   NiFpga_ts_M1M3SupportFPGA_IndicatorArrayU64_Rx = 0x18004,
   NiFpga_ts_M1M3SupportFPGA_IndicatorArrayU64_Tx = 0x18000,
} NiFpga_ts_M1M3SupportFPGA_IndicatorArrayU64;

typedef enum
{
   NiFpga_ts_M1M3SupportFPGA_IndicatorArrayU64Size_Rx = 5,
   NiFpga_ts_M1M3SupportFPGA_IndicatorArrayU64Size_Tx = 5,
} NiFpga_ts_M1M3SupportFPGA_IndicatorArrayU64Size;

typedef enum
{
   NiFpga_ts_M1M3SupportFPGA_TargetToHostFifoU8_U8ResponseFIFO = 0,
} NiFpga_ts_M1M3SupportFPGA_TargetToHostFifoU8;

typedef enum
{
   NiFpga_ts_M1M3SupportFPGA_TargetToHostFifoU16_U16ResponseFIFO = 1,
} NiFpga_ts_M1M3SupportFPGA_TargetToHostFifoU16;

typedef enum
{
   NiFpga_ts_M1M3SupportFPGA_HostToTargetFifoU16_CommandFIFO = 4,
   NiFpga_ts_M1M3SupportFPGA_HostToTargetFifoU16_RequestFIFO = 3,
} NiFpga_ts_M1M3SupportFPGA_HostToTargetFifoU16;

typedef enum
{
   NiFpga_ts_M1M3SupportFPGA_HostToTargetFifoU64_TimestampControlFIFO = 2,
} NiFpga_ts_M1M3SupportFPGA_HostToTargetFifoU64;


#if NiFpga_Cpp
}
#endif

#endif
