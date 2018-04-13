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
static const char* const NiFpga_M1M3Support_Signature = "A5C809822CAE7FA501C39E768256AF40";

typedef enum
{
   NiFpga_M1M3Support_TargetToHostFifoU8_U8ResponseFIFO = 0,
} NiFpga_M1M3Support_TargetToHostFifoU8;

typedef enum
{
   NiFpga_M1M3Support_TargetToHostFifoU16_U16ResponseFIFO = 1,
} NiFpga_M1M3Support_TargetToHostFifoU16;

typedef enum
{
   NiFpga_M1M3Support_HostToTargetFifoU16_CommandFIFO = 4,
   NiFpga_M1M3Support_HostToTargetFifoU16_RequestFIFO = 3,
} NiFpga_M1M3Support_HostToTargetFifoU16;

typedef enum
{
   NiFpga_M1M3Support_HostToTargetFifoU64_TimestampControlFIFO = 2,
} NiFpga_M1M3Support_HostToTargetFifoU64;

#endif
