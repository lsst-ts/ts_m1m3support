/*
 * Generated with the FPGA Interface C API Generator 16.0.0
 * for NI-RIO 16.0.0 or later.
 */

#ifndef __NiFpga_M1M3Expansion_h__
#define __NiFpga_M1M3Expansion_h__

#ifndef NiFpga_Version
   #define NiFpga_Version 1600
#endif

#include "NiFpga.h"

/**
 * The filename of the FPGA bitfile.
 *
 * This is a #define to allow for string literal concatenation. For example:
 *
 *    static const char* const Bitfile = "C:\\" NiFpga_M1M3Expansion_Bitfile;
 */
#define NiFpga_M1M3Expansion_Bitfile "NiFpga_M1M3Expansion.lvbitx"

/**
 * The signature of the FPGA bitfile.
 */
static const char* const NiFpga_M1M3Expansion_Signature = "B0AE4278BEACC85330A5DB1C38E9AE71";

typedef enum
{
   NiFpga_M1M3Expansion_IndicatorU32_Slot2 = 0x18000,
} NiFpga_M1M3Expansion_IndicatorU32;

typedef enum
{
   NiFpga_M1M3Expansion_ControlBool_Sample = 0x1800A,
} NiFpga_M1M3Expansion_ControlBool;

typedef enum
{
   NiFpga_M1M3Expansion_IndicatorArraySgl_Slot1 = 0x18004,
} NiFpga_M1M3Expansion_IndicatorArraySgl;

typedef enum
{
   NiFpga_M1M3Expansion_IndicatorArraySglSize_Slot1 = 6,
} NiFpga_M1M3Expansion_IndicatorArraySglSize;

#endif
