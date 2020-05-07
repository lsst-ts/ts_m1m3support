#ifndef LSST_M1M3_SS_FPGA_NiError_H_
#define LSST_M1M3_SS_FPGA_NiError_H_

#include "NiFpga.h"
#include <stdexcept>

/**
 * Error thrown on FPGA problems. Provides what routine with explanatory
 * string.
 */
class NiError : public std::runtime_error {
public:
    /**
     * Construct the error.
     *
     * @param status Ni error status.
     */
    NiError(const char *msg, NiFpga_Status status);

private:
    NiFpga_Status status;
};

/**
 * Throws NI exception if an error occurred.
 */
void NiThrowError(const char *msg, int32_t status);

#endif  // !LSST_M1M3_SS_FPGA_NiError_H_
