#include "NiError.h"
#include "NiStatus.h"

NiError::NiError(const char *msg, NiFpga_Status status)
        : std::runtime_error(NiStatus(status)), status(status) {
    NiReportError(msg, status);
}

void NiThrowError(const char *msg, int32_t status) {
    if (status != 0) {
        throw NiError(msg, status);
    }
}
