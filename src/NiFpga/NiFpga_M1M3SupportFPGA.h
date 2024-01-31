/*
 * Generated with the FPGA Interface C API Generator 19.0
 * for NI-RIO 19.0 or later.
 */
#ifndef __NiFpga_M1M3SupportFPGA_h__
#define __NiFpga_M1M3SupportFPGA_h__

#ifndef NiFpga_Version
#define NiFpga_Version 190
#endif

#include "NiFpga.h"

/**
 * The filename of the FPGA bitfile.
 *
 * This is a #define to allow for string literal concatenation. For example:
 *
 *    static const char* const Bitfile = "C:\\" NiFpga_M1M3SupportFPGA_Bitfile;
 */
#define NiFpga_M1M3SupportFPGA_Bitfile "NiFpga_M1M3SupportFPGA.lvbitx"

/**
 * The signature of the FPGA bitfile.
 */
static const char *const NiFpga_M1M3SupportFPGA_Signature = "872665D8D22D4D39C899121ADF6983DB";

#if NiFpga_Cpp
extern "C" {
#endif

typedef enum {
    NiFpga_M1M3SupportFPGA_TargetToHostFifoU8_U8ResponseFIFO = 0,
} NiFpga_M1M3SupportFPGA_TargetToHostFifoU8;

typedef enum {
    NiFpga_M1M3SupportFPGA_TargetToHostFifoU16_U16ResponseFIFO = 1,
} NiFpga_M1M3SupportFPGA_TargetToHostFifoU16;

typedef enum {
    NiFpga_M1M3SupportFPGA_TargetToHostFifoU64_HealthAndStatusDataFIFO = 5,
} NiFpga_M1M3SupportFPGA_TargetToHostFifoU64;

typedef enum {
    NiFpga_M1M3SupportFPGA_HostToTargetFifoU16_CommandFIFO = 7,
    NiFpga_M1M3SupportFPGA_HostToTargetFifoU16_HealthAndStatusControlFIFO = 6,
    NiFpga_M1M3SupportFPGA_HostToTargetFifoU16_RequestFIFO = 3,
} NiFpga_M1M3SupportFPGA_HostToTargetFifoU16;

typedef enum {
    NiFpga_M1M3SupportFPGA_HostToTargetFifoU64_TimestampControlFIFO = 2,
} NiFpga_M1M3SupportFPGA_HostToTargetFifoU64;

#if !NiFpga_VxWorks

/* Indicator: Meantemperature */
const NiFpga_FxpTypeInfo NiFpga_M1M3SupportFPGA_IndicatorFxp_Meantemperature_TypeInfo = {1, 16, 10};

/* Use NiFpga_ReadU16() to access Meantemperature */
const uint32_t NiFpga_M1M3SupportFPGA_IndicatorFxp_Meantemperature_Resource = 0x18002;

/* FIFO: RawAccelerometer */
const NiFpga_FxpTypeInfo NiFpga_M1M3SupportFPGA_TargetToHostFifoFxp_RawAccelerometer_TypeInfo = {1, 24, 5};

/* Use NiFpga_ReadFifoU64() to access RawAccelerometer */
const uint32_t NiFpga_M1M3SupportFPGA_TargetToHostFifoFxp_RawAccelerometer_Resource = 4;

#endif /* !NiFpga_VxWorks */

#if NiFpga_Cpp
}
#endif

#endif
